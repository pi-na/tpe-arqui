#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <keyboard.h>

static void int_20();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
	}
	return;
}

void int_20() {
	timer_handler();
	char * c = tryKeyboard();
	if (*c == '\n'){
		ncNewline();
	} else {
		ncPrint(c);
	}

	return;
}
