#include <shell.h>
#include <libc.h>
#include <commands.h>
#include <clock.h>

static int  startedShell = 0;
static char username[] = "User";
static char commandList[][30] = {"help","divzero","inforeg","clear","clock","snake","opcode", 0};
void (*commandFuncts[])(void*) = {help,divzero,registersinfo,clearScreen,rtclock,snake,opCode};

void startShell()
{
    setUserLength(username);
    welcomeMessage();
    startedShell = 1;
    runShell();
}

void userPrint(){
    printf("\n %s > ", username);
}


static int getCommandArgs(char* userInput, char* command, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
    int argc = 0;
    int i = 0;
    for(i = 0; userInput[i] != 0 && userInput[i] != ' '; i++) {
        command[i] = userInput[i];
    }
    
    command[i] = 0;
    int argIdx = 0;

    for(int j = 0; j < MAX_ARGUMENTS; j++) {
            argv[j][0] = 0;
    }

    while(userInput[i] != 0 && argc < MAX_ARGUMENTS) {
        i++;
        argIdx = 0;
        for(; userInput[i] != ' ' && userInput[i] != 0; i++, argIdx++) {
           argv[argc][argIdx] = userInput[i];
        }
        argv[argc][argIdx] = 0;
        argc++;
    }
    if(argc == MAX_ARGUMENTS && userInput[i] != 0)
        return -1;
    return argc;
}

void welcomeMessage(){
    if(startedShell>0){
        return;
    }
    logo();
}
 

void cnf(){
    printf("\ncommand not found\n\nType 'help' to see available commands\n");
}

void runShell(){
    while (1)
    {
        playSound(100);
        char command[BUFFER_SIZE] = {0};
        char userInput[BUFFER_SIZE] = {0};
        char argv[MAX_ARGUMENTS][BUFFER_SIZE];
        int argc = 0;
        int inputSize = 0;
        char c=0;
        userPrint();
        while (c != '\n' )
        {
            c = readChar();
            if(c!='\n' && c!=0){
                if(c == '\b'){
                    userInput[--inputSize]=0;
                    //inputSize--;
                }else{
                    userInput[inputSize++] = c;
                }
            }
            putChar(c);
        }
        if(inputSize==0){
            cnf();
        }else{
            int found = 0;
            argc = getCommandArgs(userInput, command, argv);
            if(argc == -1) {
                printf("\nToo many arguments\nmax allowed arguments: %d.\n\n", MAX_ARGUMENTS);
            }

            for (int i = 0; commandList[i][0]!=0; i++)
            {
                if(strcmp(command,commandList[i])==0){
                    int arg=0;
                    if(argc > 0){
                        char arg2 = argv[0][0];
                        arg = arg2-'0';
                    }
                    commandFuncts[i](arg);
                    found = 1;
                }
            }
            if(found == 0){
                cnf();
            }
        }
    }
}


void logo(){
    printf("\n");
    printf("                                                                      :\n");
    printf("                                                                       :\n");
    printf("                                                                     ::\n");
    printf("                                                                     :::\n");
    printf("                                                                     ::::\n");
    printf("                                                                    .::::.\n");
    printf("                                                                    ::::::.\n");
    printf("                                                                   :::::::::\n");
    printf("                                                                  :::::::::::                                           .\n");
    printf("                                                                 :::::::::::::                                         ::\n");
    printf("                                                                :::::::::::::::                                      .::\n");
    printf("                                                              :::::::::::::::::                                    .:::\n");
    printf("                                                             ::::::::::::::::::                                  .:::::\n");
    printf("                                                            :::::::::::::::''''                                .::::::\n");
    printf("                                                           :::::::::::',,cd$$$$$$$$$$hcc,.                  .:::::::::\n");
    printf("                                                         :::::::'`.,cc$$$$$$$$$$$$$$$$$$$$$$c,.         ..:::::::::::'\n");
    printf("                                                        :::::: ,c$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$h   ..::::::::::::::::\n");
    printf("                                                       ::::: ,$$P\"\',$$P\"\"$$$$$$$$$$$$$$$$$$PF\"\"..::::::::::::::::::::\n");
    printf("                                                       ::: ,$$\",c$$$\",d$$$$$$$$$$$$$$$P\"\".::::::::::::::::::::::::::\n");
    printf("                                                      ::: J$$$$$$$$$$$$$$$$$$$$$$P\"\'.:::::::::::::::::::::::::::::::\n");
    printf("                                                     ::: $$$$$$$$$$$$$$$$$$$$P\".::::::::::::::::::::::::::::::::::::\n");
    printf("                                                     :: J$$$$$$$$$$$$$$$$$$P\':::::::::::::::::::::::::::::::::::::::\n");
    printf("                                                     :: ?$$$$$$$$$$$$$$$P\".:::::::::::::::::::::::::::::::::::::::::\n");
    printf("       Wolverine OS                                 .::: $$$$$$$$$$$$$\".::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("                                                    `::::`?$$$$\"3$$$$\':::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("                                                     ::::: ?$$F,$$$$':::::::::::::::::::::::::::::::::::`:::::::::'\n");
    printf("                                                     :::::: \"P $F3$\'::::::::::::::::::::::::::::::`. '`:: `::::::'\n");
    printf("                                                    ```:::::: <$ $$ ::::::::````:::::::::::::::::` :: : :: :::::\n");
    printf("       Press ENTER to start                            =cc,`:  \" \"\" :::\'\`,cc$$$ `::::::::::::::::::`:`  :::::::\n");
    printf("                                                       :.`"" d$$$$$ ::  `???"".::::::::::::::::::::` : :: ::'`\n");
    printf("                                                      ::::: d$$$$F :::::::::::::::::::::::::::::::::: :: ',,cc,\n");
    printf("                                                      ::::'J$$$$$> ::::::::::::::::::::::::::::::::::'`,c$$$$$$$h,\n");
    printf("                                                      `::: $$$$P'::::::::::::::::::::::::::::::::::::<$$$$$$$$$$$$$,?$$$ccccc,.\n");
    printf("                                                         ` $$$P :::::::::``````::::::::::::::::::::::<$$$$$$$$$$$$$$cJ$$$$$$$$$\n");
    printf("                                                        '$ ??\" `.,,cccccc$$$$$$ccc,``::::::::::::::: J$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("                                                         $$hh$$$$$$$$$$h,\"?$$$$$$$$$.`::::::::::::: c$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("                                                         ?$??$$?????$$$$$$$c.`\"$$$$$ ::::::::::::: ,$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("                                                          $$ec :;;;;;;;, \"$$$$c $$$$ :::::::::::: J$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("                                                          $$$$,,`'''`,,ccc$$$$$,\"$$$> :::::::``  $$$$$$$$$$$$$$$$$$$$$$$$$$$$P\"\n");
    printf("                                                          4$\",$$$$$$$$$$$$$$$$$$c$$$\':::``,ccc$  $$$$$$$$$$$$$$$$$$$$$$$$$$$\".:\n");
    printf("                                                          4$$cccc,,$$$$$$$$$$$$$$??\".,,c$$$$$$$  $$$$$$$$$$$$$$$$$$F,z$$$$\" :::\n");
    printf("                                                        :: \"$$$$$$$$$$$$$$$$P\",, d\")C3$$$$$$$$$> $$$$$$$$$$$$$$$P\",c$$$$$\" ::::\n");
    printf("                                                       ::::.\"???$$$$$$$$$P\"..\" \"L\'$ccc?$$$$$$$$$ ?$$$$$$$$$$$$$P z$$$$$$F :::::\n");
    printf("       v0.1                                            :::::::::::.."""".::\',c  `ccc==-3$$$$$$$$h`$$$$$$$$$$$$\",,$$$$$$$ ::::::\n");
    printf("                                                       :::::::::::::::::::\',$F\': .`??\",`?$$$$$$$$c$$$$$$$$$$$F, d$$$$$$F ::::::\n");
    printf("                                                        :::::::::::::::::: $$F::::`?cc,\"?$$$$$$$$$$$$$$$$$$$$,L$$$$$$$$ ::::```\n");
    printf("                                                         ::::::::::::::::\'J$$\':::: zc,\"=-\"\"?$$$$$$$$$$$$$$$$$$$$$$$PF\" .::::::\n");
    char c;
    while (c != '\n'){
        c = readChar();
    }
    wipeScreen();
}
