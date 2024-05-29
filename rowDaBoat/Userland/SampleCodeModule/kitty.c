#include <usr_stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys_calls.h>
#include <colors.h>
#include "eliminator.h"
#include "kitty.h"

#define MAX_BUFF 254
#define MAX_ARGS 10
#define USERNAME_SIZE 16

char line[MAX_BUFF+1] = {0}; //asi me aseguro que al menos va haber un cero
char parameter[MAX_BUFF+1] = {0};
char command[MAX_BUFF+1] = {0};
int linePos = 0;
char lastc;
static char username[USERNAME_SIZE] = "user";
char usernameLength = 4;

void printHelp(){
	prints("\n-help-               displays current menu",MAX_BUFF);
	prints("\n-time-               display current time",MAX_BUFF);
	prints("\n-clear-              clear the display",MAX_BUFF);
	prints("\n-terminator-         launch TERMINATOR videogame",MAX_BUFF);
	prints("\n-inforeg-            print current register values",MAX_BUFF);
	prints("\n-zerodiv-            testeo divide by zero exception",MAX_BUFF);
	prints("\n-invopcode-          testeo invalid op code exception",MAX_BUFF);
	prints("\n-(+)-                increase font size (scaled)",MAX_BUFF);
	prints("\n-(-)-                decrease font size (scaled)",MAX_BUFF);
	prints("\n-setusername         set username",MAX_BUFF);
	printc('\n');
}

// static Color BLACK = {0,0,0};
// static Color WHITE = {255,255,255};
// static Color RED = {0,0,255};
// static Color LIGHT_BLUE = {255,255,255};
// static Color BLUE = {255,0,0};
// static Color ORANGE = {16,160,255};
// static Color YELLOW = {30,224,255};
// static Color PURPLE = {255,32,160};
// static Color PINK = {100,0,244};

static void newLine();
static void printLine(char c);
static int checkLine();
static void cmd_undefined();
static void cmd_help();
static void cmd_time();
static void cmd_clear();
static void cmd_inforeg();
static void cmd_zeroDiv();
static void cmd_invOpcode();
static void cmd_charsizeplus();
static void cmd_charsizeminus();
static void cmd_setusername();

const char * commands[] = {"undefined","help","time","clear","inforeg","zerodiv","invopcode","setusername"};
static void (*commands_ptr[MAX_ARGS])() = {cmd_undefined, cmd_help, cmd_time, cmd_clear, cmd_inforeg, cmd_zeroDiv,cmd_invOpcode, cmd_setusername};

void kitty (){
	char c;
	printc('$');
	prints(username, usernameLength);
	printc('>');

	while(1){
		c = getChar();
		switch (c){
			case '+':
				cmd_charsizeplus();
				break;
			case '-':
				cmd_charsizeminus();
				break;
			default:
				printLine(c);
		};
	}
}

static void printLine(char c){
	if (linePos < MAX_BUFF && c != lastc){
		if (isChar(c) || c == ' ' ||isDigit(c)){
			line[linePos++] = c;
			printc(c);
		} else if (c == '\b' && linePos > 0){
			printc(c);
			line[--linePos] = 0;
		} else if (c == '\n'){
			newLine();
		}
	}
	lastc = c;
}


static void newLine(){
	int i = checkLine();

	(*commands_ptr[i])();

	for (int i = 0; line[i] != '\0' ; i++){
		line[i] = 0;
		command[i] = 0;
		parameter[i] = 0;
	}
	linePos = 0;

	if (i != 3 ){
		prints("\n$",MAX_BUFF);
		prints(username, usernameLength);
		printc('>');
	} else {
		printc('$');
		prints(username, usernameLength);
		printc('>');
	}
}


//separa comando de parametro
static int checkLine(){
	int i = 0;
	int j = 0;
	int k = 0;
	for ( j = 0 ; j < linePos && line[j] != ' ' ; j ++){
		command[j] = line[j];
	}
	if (j < linePos){
		j++;
		while (j < linePos){
			parameter[k++] = line[j++];
		}
	}



	for (i = 1 ; i < MAX_ARGS ; i++ ){
		if (strcmp(command,commands[i]) == 0){
			return i;
		}
	}

	return 0;
}

static void cmd_setusername(){
	prints("\nEnter new username (press ENTER to finish): ",MAX_BUFF);
	int i = 0;
 	char c;
	while (i < 15){
		c = getChar();
		if (isChar(c)){
			username[i++] = c;
			printc(c);
		} else if (c == '\n'){
			break;
		}
	}
	if(i < 3){
		prints("\nUsername must be at least 3 characters long! Username not set.",MAX_BUFF);
	}
	username[i] = 0;
	usernameLength = i;
	prints("Username set to ", MAX_BUFF);
	prints(username, USERNAME_SIZE);
}

static void cmd_help(){
	prints("\n===== Displaying PIBES OS command list =====\n",MAX_BUFF);
	printHelp();
}

static void cmd_undefined(){
	prints("\n\ncommand not found:\"",MAX_BUFF);
	prints(command,MAX_BUFF);
	prints("\" Use help to display available commands",MAX_BUFF);
}

static void cmd_time(){
	getTime();
}

static void cmd_clear(){
	clear_scr();
}

static void cmd_inforeg(){
	inforeg();
}

static void cmd_invOpcode(){
	test_invopcode();
}

static void cmd_zeroDiv(){
	test_zerodiv();
}

static void cmd_charsizeplus(){
	increaseScale();
}

static void cmd_charsizeminus(){
	decreaseScale();
}





