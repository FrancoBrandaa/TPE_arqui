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

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();
//PARTE DE LOS EJERICIOS
void imprimir_arqui();
//void print(char* message);
void vga_puts(const char* str);

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

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
	//clave no olvidar
	load_idt(); //cargo la idt para cuando haya una interrupcion labure lo que quiero

	
	// ncPrint("[Kernel Main]");
	// ncNewline();
	// ncPrint("  Sample code module at 0x");
	// ncPrintHex((uint64_t)sampleCodeModuleAddress);
	// ncNewline();
	// ncPrint("  Calling the sample code module returned: ");
	// ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	// ncNewline();
	// ncNewline();

	// ncPrint("  Sample data module at 0x");
	// ncPrintHex((uint64_t)sampleDataModuleAddress);
	// ncNewline();
	// ncPrint("  Sample data module contents: ");
	// ncPrint((char*)sampleDataModuleAddress);
	// ncNewline();

	// ncPrint("[Finished]");
	EntryPoint userMain = (EntryPoint)sampleCodeModuleAddress;
	userMain();

	ncPrint("<<< El control volvió del userland >>>");  // Esto no debería aparecer

	//imprimir_arqui();
	//vga_puts("habia una vez una vaca sentada en un plato de madera");


	// EJERCICIO 3 --------------------------------------//
	// while(1){
	
	// ncClear();
	// ncPrint("HORAS: ");
	// char a = read_hours();
	// ncPrintHex(a);
	// ncPrint(" MINUTOS: ");
	// char b = read_minutes();
	// ncPrintHex(b);
	// ncPrint(" SEGUNDOS: ");
	// char c = read_sec(); 
	// ncPrintHex(c);   //  basicamente me lo dan en exa el resultado
	// //ncNewline();
	// }
	// --------------------------------------------------//
	
	//print("habia una vez una vaa sentada en un plato de madera");
	while(1);
	return 0;
}
