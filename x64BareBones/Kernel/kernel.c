#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <defs.h>
#include <time.h>
#include <keyboardDriver.h>
#include<soundDriver.h>
#include <videoDriver.h>
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void test_write();
extern char read_minutes();
extern char read_sec();
extern char read_hours();

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000; //en que direccion vamos a userland
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

//void print(char* message);
//void vga_puts(const char* str);  

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}
/*
reserva 32 KiB inmediatamente tras tu kernel y devuelve la dirección inicial de la pila (alineada al tope)
,para que al arrancar el CPU comience a utilizar ese espacio de forma segura.
*/
void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	
	
	//Carga la Interrupt Descriptor Table (IDT)
	load_idt(); //Manejo de interrupciones y syscalls
	drawCircle(100,100,100,0x00FF0000);
	playTone(200,4000); //para testing
	//Realiza un salto al código de userland
	((EntryPoint)sampleCodeModuleAddress)();
	haltcpu();
	return 0;	
}
