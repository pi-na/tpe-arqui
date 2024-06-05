#include <sys_calls.h>
#include <userlib.h>

int getHours()
{
	return sys_getHours();
}

int getMinutes()
{
	return sys_getMinutes();
}

int getSeconds()
{
	return sys_getSeconds();
}

void getTime()
{
	int hours, minutes, seconds;

	hours = getHours();
	minutes = getMinutes();
	seconds = getSeconds();

	printc('\n');
	hours = hours - 3;
	printDec(hours);
	printc(':');
	printDec(minutes);
	printc(':');
	printDec(seconds);
}
