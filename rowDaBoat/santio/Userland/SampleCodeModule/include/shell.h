#ifndef SHELL_H
#define SHELL_H

#define USER_SIZE 25
#define BUFFER_SIZE 100
#define MAX_ARGUMENTS 3

void help();
void welcomeMessage();
void startShell();
void logo();
char readChar();
void runShell();

#endif