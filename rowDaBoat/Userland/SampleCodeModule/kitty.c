#include <userlib.h>
#include <stdio.h>
#include <time.h>
#include <sys_calls.h>
#include <colors.h>
#include "eliminator.h"
#include "kitty.h"

#define MAX_BUFF 254
#define MAX_COMMAND 16
#define MAX_ARGS 10
#define USERNAME_SIZE 16
#define NEW_LINE '\n'
#define BACKSPACE '\b'
#define PLUS '+'
#define MINUS '-'



//initialize all to 0
char line[MAX_BUFF+1] = {0}; 
char parameter[MAX_BUFF+1] = {0};
char command[MAX_BUFF+1] = {0};
int linePos = 0;
char lastc;
static char username[USERNAME_SIZE] = "user";
static char commandBuffer[MAX_COMMAND][MAX_BUFF] = {0};
static int commandIterator = 0;
static int commandIdxMax = 0;

char usernameLength = 4;

void printHelp(){
	prints("\n>help                - displays this shell information",MAX_BUFF);
	prints("\n>setusername         - set username",MAX_BUFF);
	prints("\n>whoami              - display current username",MAX_BUFF);
	prints("\n>time               - display current time",MAX_BUFF);
	prints("\n>clear              - clear the display",MAX_BUFF);
	prints("\n>(+)                - increase font size (scaled)",MAX_BUFF);
	prints("\n>(-)                - decrease font size (scaled)",MAX_BUFF);
	prints("\n>inforeg            - print current register values",MAX_BUFF);
	prints("\n>zerodiv            - testeo divide by zero exception",MAX_BUFF);
	prints("\n>invopcode          - testeo invalid op code exception",MAX_BUFF);
	prints("\n>terminator         - launch TERMINATOR videogame",MAX_BUFF);
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
static void printPrompt();
static void cmd_whoami();
static void historyCaller(int direction);


const char * commands[] = {"undefined","help","time","clear","inforeg","zerodiv","invopcode","setusername","whoami"};
static void (*commands_ptr[MAX_ARGS])() = {cmd_undefined, cmd_help, cmd_time, cmd_clear, cmd_inforeg, cmd_zeroDiv, cmd_invOpcode, cmd_setusername, cmd_whoami};

void kitty (){
	char c;
	printPrompt();

	while(1){
		drawCursor();
		c = getChar();
		printLine(c);
	}
}

static void printLine(char c){
	if (linePos < MAX_BUFF && c != lastc){
		if (isChar(c) || c == ' ' || isDigit(c)){
			if (c == PLUS){
				cmd_charsizeplus();
			}else if (c == MINUS){
				cmd_charsizeminus();
			}else if (isUpperArrow(c)){
				historyCaller(-1);
			}else if (isDownArrow(c)){
				historyCaller(1);
			}else{
				line[linePos++] = c;
				printc(c);
			}
		} else if (c == BACKSPACE && linePos > 0){
			printc(c);
			line[--linePos] = 0;
		} else if (c == NEW_LINE){
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

	prints("\n",MAX_BUFF);
	printPrompt();
}

static void printPrompt(){
	prints(username, usernameLength);
	prints(" $",MAX_BUFF);
	printc('>');
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

	strcpy(commandBuffer[commandIdxMax++],command);
	commandIterator = commandIdxMax;

	for (i = 1 ; i < MAX_ARGS ; i++ ){
		if (strcmp(command,commands[i]) == 0){
			return i;
		}
	}

	return 0;
}



static void cmd_setusername(){
	int input_length = strlen(parameter);
	if(input_length < 3 || input_length > USERNAME_SIZE){
		prints("\nERROR: Username length must be between 3 and 16 characters long! Username not set.", MAX_BUFF);
		return;
	}
	usernameLength = input_length;
	for(int i = 0; i < input_length; i++){
		username[i] = parameter[i];
	}
	prints("\nUsername set to ", MAX_BUFF);
	prints(username, usernameLength);
}

static void cmd_whoami(){
	prints("\n",MAX_BUFF);
	prints(username, usernameLength);
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
	cmd_clear();
	increaseScale();
	printPrompt();
}

static void cmd_charsizeminus(){
	cmd_clear();
	decreaseScale();
	printPrompt();
}

static void historyCaller(int direction){
	cmd_clear();
	printPrompt();
	commandIterator += direction;
	prints(commandBuffer[commandIterator],MAX_BUFF);
	strcpy(line,commandBuffer[commandIterator]);
	linePos = strlen(commandBuffer[commandIterator]);
}





