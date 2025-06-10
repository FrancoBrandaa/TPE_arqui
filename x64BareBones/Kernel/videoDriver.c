#include <videoDriver.h>

static int cursorX = 0;
static int cursorY = 0;

#define CHAR_HEIGHT 16
#define CHAR_SPACING 12
#define LINE_HEIGHT 16
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define FG_COLOR 0xFFFFFF
#define BG_COLOR 0x000000

#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_RED 0xFF0000

// Double buffer
#define BUFFER_WIDTH 1920
#define BUFFER_HEIGHT 1080

static uint32_t backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

uint32_t color = COLOR_WHITE;
uint32_t backgroundColor = COLOR_BLACK;
uint8_t zoom = 2;

struct vbe_mode_info_structure
{
    uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;     // deprecated
    uint8_t window_b;     // deprecated
    uint16_t granularity; // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;        // number of bytes per horizontal line
    uint16_t width;        // width in pixels
    uint16_t height;       // height in pixels
    uint8_t w_char;        // unused...
    uint8_t y_char;        // ...
    uint8_t planes;
    uint8_t bpp;   // bits per pixel in this mode
    uint8_t banks; // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;
    uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__((packed));


typedef struct vbe_mode_info_structure *VBEInfoPtr;
VBEInfoPtr VBE_mode_info = (VBEInfoPtr)0x0000000000005C00;




int getVbeInfo(neededInfo info){
    if (VBE_mode_info == NULL) {
        // Manejo de error: VBE_mode_info no está inicializado
        return  1; 
    }
    info->width = VBE_mode_info->width;
    info->height = VBE_mode_info->height;
    return 0;
}

// Función para copiar el back buffer al front buffer (swap)
void swapBuffers(void)
{
    uint8_t *framebuffer = (uint8_t *)(uintptr_t)VBE_mode_info->framebuffer;
    uint64_t screenWidth = VBE_mode_info->width;
    uint64_t screenHeight = VBE_mode_info->height;
    uint8_t bpp = VBE_mode_info->bpp / 8;
    uint32_t pitch = VBE_mode_info->pitch;

    // Copiar solo la parte visible del back buffer a la pantalla
    for (uint64_t y = 0; y < screenHeight && y < BUFFER_HEIGHT; y++) {
        for (uint64_t x = 0; x < screenWidth && x < BUFFER_WIDTH; x++) {
            uint32_t pixel = backBuffer[y * BUFFER_WIDTH + x];
            uint64_t offset = x * bpp + y * pitch;
            
            framebuffer[offset] = pixel & 0xFF;
            framebuffer[offset + 1] = (pixel >> 8) & 0xFF;
            framebuffer[offset + 2] = (pixel >> 16) & 0xFF;
        }
    }
}

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y)
{
        if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
        backBuffer[y * BUFFER_WIDTH + x] = hexColor;
    }
}

void vd_cleanScreen(void)
{

        for (int i = 0; i < BUFFER_WIDTH * BUFFER_HEIGHT; i++) {
        backBuffer[i] = backgroundColor;
    }

    // Reset cursor position
    cursorX = 0;
    cursorY = 0;
}

void vd_setFontColor(uint32_t hexColor)
{
    uint32_t oldColor = color;
    color = hexColor;
    replaceColor(oldColor, hexColor);
}

void replaceColor(uint32_t oldColor, uint32_t newColor)
{
        // Reemplazar en el back buffer
        for (int i = 0; i < BUFFER_WIDTH * BUFFER_HEIGHT; i++) {
            if (backBuffer[i] == oldColor) {
                backBuffer[i] = newColor;
            }
        }
}

void vd_setCursor(int x, int y)
{
    cursorX = CHAR_WIDTH * x * zoom;
    cursorY = CHAR_HEIGHT * y * zoom;
}

void vd_setZoom(int new_zoom)
{
    zoom = new_zoom;
}

void vd_SetBackgroundColor(uint32_t hexColor)
{
    backgroundColor = hexColor;
}


void vd_printstr(FDS fd, const char *buf, size_t count)
{
    int i = 0;
    while (i < count)
    {
        vd_putChar(*buf++, fd);
        i++;
    }
}

static void putMultPixel(uint32_t hexColor, uint64_t x, uint64_t y, int mult)
{
    for (int i = 0; i < mult; i++)
    {
        for (int j = 0; j < mult; j++)
        {
            putPixel(hexColor, x + i, y + j);
        }
    }
}

void vd_putChar(char c, FDS fd)
{

    if (c == '\n')
    { // new line
        cursorX = 0;
        cursorY += CHAR_HEIGHT * zoom;
    }
    else if (c == '\b')
    { // delete
        if (cursorX >= CHAR_WIDTH * zoom)
        {
            cursorX -= CHAR_WIDTH * zoom;
            drawChar(' ', cursorX, cursorY, color, backgroundColor, zoom);
        }
        else if (cursorX == 0 && cursorY >= CHAR_HEIGHT * zoom)
        {
            cursorY -= CHAR_HEIGHT * zoom;
            cursorX = (VBE_mode_info->width / (CHAR_WIDTH * zoom)) * CHAR_WIDTH * zoom;
            drawChar(' ', cursorX, cursorY, color, backgroundColor, zoom);
        }
    }
    else
    {
        drawChar(c, cursorX, cursorY, color, backgroundColor, zoom);
        cursorX += CHAR_WIDTH * zoom;
    }

    // check if cursorX exceeds screen width
    if (cursorX + CHAR_WIDTH * zoom > VBE_mode_info->width)
    {
        cursorX = 0;
        cursorY += CHAR_HEIGHT * zoom;
    }

    // check if cursorY exceeds screen height
    if (cursorY + CHAR_HEIGHT * zoom > VBE_mode_info->height)
    {
        scroll_screen();
        cursorY -= CHAR_HEIGHT * zoom;
    }
}

void drawChar(unsigned char c, int x, int y, int fgcolor, int bgcolor, int mult)
{
    int mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
    unsigned char *glyph = font_bitmap + (int)c * 16;

    for (int cy = 0; cy < CHAR_HEIGHT * mult; cy += mult)
    {
        for (int cx = 0; cx < CHAR_WIDTH * mult; cx += mult)
        {
            putMultPixel(glyph[cy / mult] & mask[cx / mult] ? fgcolor : bgcolor, (x + cx), (y + cy), mult);
        }
    }
}



void drawCircle(int center_x, int center_y, int radius, uint32_t color) {
// Boundary check
    if (center_x + radius < 0 || center_x - radius >= VBE_mode_info->width ||
        center_y + radius < 0 || center_y - radius >= VBE_mode_info->height) {
        return;
    }

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            // Check if point is inside circle using distance formula
            if (x * x + y * y <= radius * radius) {
                int px = center_x + x;
                int py = center_y + y;
                
                // Bounds checking
                if (px >= 0 && px < VBE_mode_info->width && 
                    py >= 0 && py < VBE_mode_info->height) {
                    putPixel(color, px, py);
                }
            }
        }
    }
}


void scroll_screen(void)
{
        // Scroll en el back buffer
        uint32_t lines = CHAR_HEIGHT * zoom;
        uint32_t bytesToMove = (BUFFER_HEIGHT - lines) * BUFFER_WIDTH * sizeof(uint32_t);
        
        // Mover líneas hacia arriba
        for (uint32_t y = 0; y < BUFFER_HEIGHT - lines; y++) {
            for (uint32_t x = 0; x < BUFFER_WIDTH; x++) {
                backBuffer[y * BUFFER_WIDTH + x] = backBuffer[(y + lines) * BUFFER_WIDTH + x];
            }
        }
        
        // Limpiar las líneas de abajo
        for (uint32_t y = BUFFER_HEIGHT - lines; y < BUFFER_HEIGHT; y++) {
            for (uint32_t x = 0; x < BUFFER_WIDTH; x++) {
                backBuffer[y * BUFFER_WIDTH + x] = backgroundColor;
            }
        }
}



