#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
<<<<<<< HEAD
#include <videoDriver.h>
=======
#include <keyboard.h>
>>>>>>> 4537f8376d958641e3ab0c162c7d8ed52de5a028

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize) {
    memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
    return (void*)((uint64_t)&endOfKernel + PageSize * 8 - sizeof(uint64_t));
}

void * initializeKernelBinary() {
    char buffer[10];

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
	load_idt();

	while(1){
		int i = 0;
		while(i++ < 100){
			putPixel(0x0000FF00, i, i);
		}
	}

	((EntryPoint)sampleCodeModuleAddress)();


	return 0;
}