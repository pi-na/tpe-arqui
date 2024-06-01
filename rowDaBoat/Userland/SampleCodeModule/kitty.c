#include <userlib.h>
#include <stdio.h>
#include <time.h>
#include <sys_calls.h>
#include <colors.h>
#include <eliminator.h>
#include <kitty.h>
#include <ascii.h>

//initialize all to 0
char line[MAX_BUFF+1] = {0}; 
char parameter[MAX_BUFF+1] = {0};
char command[MAX_BUFF+1] = {0};
int terminate = 0;
int linePos = 0;
char lastc;
static char username[USERNAME_SIZE] = "user";
static char commandBuffer[MAX_COMMAND][MAX_BUFF] = {0};
static int commandIterator = 0;
static int commandIdxMax = 0;

char usernameLength = 4;

void printHelp(){
	printsColor("\n\n    >'help' or 'ls'     - displays this shell information",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >setusername        - set username",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >whoami             - display current username",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >time               - display current time",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >clear              - clear the display",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >(+)                - increase font size (scaled)",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >(-)                - decrease font size (scaled)",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >inforeg            - print current register values",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >zerodiv            - testeo divide by zero exception",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >invopcode          - testeo invalid op code exception",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >eliminator         - launch ELIMINATOR videogame",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >whoami             - prints current username",MAX_BUFF, LIGHT_BLUE);
	printsColor("\n    >exit               - exit PIBES OS\n",MAX_BUFF, LIGHT_BLUE);

	printc('\n');
}

const char * commands[] = {"undefined", "help", "ls", "time","clear","inforeg","zerodiv","invopcode", "setusername", "whoami", "exit", "ascii", "eliminator"};
static void (*commands_ptr[MAX_ARGS])() = {cmd_undefined, cmd_help, cmd_help, cmd_time, cmd_clear, cmd_inforeg, cmd_zeroDiv, cmd_invOpcode, cmd_setusername, cmd_whoami, cmd_exit, cmd_ascii, cmd_eliminator};

void kitty (){
	char c;
	printPrompt();

	while(1 && !terminate){
		drawCursor();
		c = getChar();
		printLine(c);
	}
}

void printLine(char c) {
    if (linePos >= MAX_BUFF || c == lastc) {
        return;
    }
    if (isChar(c) || c == ' ' || isDigit(c)) {
        handleSpecialCommands(c);
    } else if (c == BACKSPACE && linePos > 0) {
        printc(c);
        line[--linePos] = 0;
    } else if (c == NEW_LINE) {
        newLine();
    }
    lastc = c;
}

void newLine(){
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

void printPrompt(){
	prints(username, usernameLength);
	prints(" $",MAX_BUFF);
	printcColor('>', PINK);
}

//separa comando de parametro
int checkLine(){
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

void cmd_setusername(){
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

void cmd_whoami(){
	prints("\n",MAX_BUFF);
	prints(username, usernameLength);
}

void cmd_help(){
	printsColor("\n\n===== Listing a preview of avaliable PIBES commands =====\n",MAX_BUFF, GREEN);
	printHelp();
}


void cmd_undefined(){
	prints("\n\nbash: command not found: \"",MAX_BUFF);
	prints(command,MAX_BUFF);
	prints("\" Use 'help' or 'ls' to display available commands",MAX_BUFF);
}

void cmd_time(){
	getTime();
}

void cmd_exit(){
	prints("\n\nExiting PIBES OS\n",MAX_BUFF);
	terminate = 1;
}

void cmd_clear(){
	clear_scr();
}

void cmd_inforeg(){
	inforeg();
}

void cmd_invOpcode(){
	test_invopcode();
}

void cmd_zeroDiv(){
	test_zerodiv();
}

void cmd_charsizeplus(){
	cmd_clear();
	increaseScale();
	printPrompt();
}

void cmd_charsizeminus(){
	cmd_clear();
	decreaseScale();
	printPrompt();
}

void handleSpecialCommands(char c) {
    if (c == PLUS) {
        cmd_charsizeplus();
    } else if (c == MINUS) {
        cmd_charsizeminus();
    } else if (isUpperArrow(c)) {
        historyCaller(-1);
    } else if (isDownArrow(c)) {
        historyCaller(1);
    } else {
        line[linePos++] = c;
        printc(c);
    }
}

void cmd_eliminator(){
	eliminator(2);
}

void historyCaller(int direction){
	cmd_clear();
	printPrompt();
	commandIterator += direction;
	prints(commandBuffer[commandIterator],MAX_BUFF);
	strcpy(line,commandBuffer[commandIterator]);
	linePos = strlen(commandBuffer[commandIterator]);
}

void cmd_ascii(){

	int asciiIdx = random();
    size_t splash_length = 0;
    while (ascii[asciiIdx][splash_length] != NULL) {
        splash_length++;
    }


	for(int i = 0; i < splash_length; i++){
		printsColor(ascii[asciiIdx][i], MAX_BUFF, WHITE);
		printc('\n');
	}
}

void welcome(){
	for (int i = 0; pibes[i] != NULL; i++){
		printsColor(pibes[i],MAX_BUFF, GREEN);
		printc('\n');
	}
	printsColor("\n    Welcome to PIBES OS, an efficient and simple operating system\n",MAX_BUFF, GREEN);
	printsColor("    Developed by the PIBES team\n",MAX_BUFF, GREEN);
	printsColor("    Here's a list of available commands\n",MAX_BUFF, GREEN);
	printHelp();
}