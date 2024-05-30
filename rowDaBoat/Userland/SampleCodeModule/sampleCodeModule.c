/* sampleCodeModule.c */
#include <sys_calls.h>
#include <stdint.h>
#include <userlib.h>
#include <kitty.h>

int main() {
	
	prints("\n\n       PIBES OS:\n",MAX_BUFF);
	prints("       Welcome to PIBES OS, an efficient and simple operating system\n",MAX_BUFF);
	prints("       Developed by the PIBES team\n",MAX_BUFF);
	prints("       Here's a list of available commands\n",MAX_BUFF);
	
	printHelp();

	kitty();

	return 0;
} 