#include <time.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <stdlib.h>

static unsigned long ticks = 0;
int se = 0;
void timer_handler()
{
	ticks++;
	se += 55;
	if (ticks % (1 * 18) == 0)
	{
	}
}

void sleep(int millis){
	se = 0;
	while (se<millis)
	{
		_hlt();
	}
}

void wait(){
	int finalTicks = ticks+9;
	while(ticks<finalTicks);
	return;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

