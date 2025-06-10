#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
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

static void * const sampleCodeModuleAddress = (void*)0xA00000; //en que direccion vamos a userland
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

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
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};
	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main()
{	
	load_idt(); //Manejo de interrupciones y syscalls
	((EntryPoint)sampleCodeModuleAddress)();
	haltcpu();
	return 0;	
}
