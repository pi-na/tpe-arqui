/* sampleCodeModule.c */
#include <sys_calls.h>
#include <stdint.h>
#include <usr_stdlib.h>
#include <kitty.h>

int main() {
	
	prints("PIBES OS:\n",MAX_BUFFER);
	prints("Welcome to PIBES OS, an efficient and simple operating system\n",MAX_BUFFER);
	prints("Developed by the PIBES team\n",MAX_BUFFER);
	prints("Here's a list of available commands\n",MAX_BUFFER);
	
	printHelp();

	kitty();

	return 0;
} 