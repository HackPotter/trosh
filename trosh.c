#include <stdio.h>
#include <stdlib.h>
#include <string.h> //necessary for string operations
#include <unistd.h> //necessary for unix syscalls
#include <wait.h> //necessary for waiting for child processes

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void defaultPrompt();
int loop();
char *getLine();

int main(int argc, char **argv){
    
    //Set up -p flag for comparing with passed flag
    char flag[3];
    strcpy(flag, "-p");
    
    //User prompt will be limited to 32 characters due to screen size
    char prompt[32];

    if(argc != 3 && argc != 1){ //either run without flags or with a prompt
        fprintf(stderr, "Useage: shell -p <prompt>\n");
        defaultPrompt(prompt);
    }
    
   //Set defalut prompt 
    if(argc == 1){
        defaultPrompt(prompt);
    }
   
   //Check for -p flag and prompt
    if(argc == 3){
        if(strcmp(flag, argv[1])){ //bad flag used
            fprintf(stderr, "Useage: shell -p <prompt>\n");
            defaultPrompt(prompt);
        }
        if(strlen(argv[2])> 31){ //Check if prompt is acceptable size
            fprintf(stderr, "Supply up to 30 chars for prompt\n");
            defaultPrompt(prompt);
        }
        else{
            strcpy(prompt, argv[2]); //Copy user prompt value into prompt array
            prompt[strlen(argv[2])] = '>'; //Append prompt with >
            prompt[strlen(argv[2]) + 1] = '\0';
        }
    }

    //loop(prompt);
    return(loop(prompt));
     

}

void defaultPrompt(char *prompt){
    strcpy(prompt, "[root@AppleWatch ß@$λ]$ ");
}

//Main loop that runs forever, returns error code if necessary to close shell
//Takes char array to use as prompt
int loop(char *prompt){
    //Built-in commands
    const char *builtIns[50];
    builtIns[0] = "exit";
    builtIns[1] = "pid";
    builtIns[2] = "ppid";
    builtIns[3] = "cd";
    builtIns[4] = "pwd";
    builtIns[5] = "set";
    builtIns[6] = "get";
    builtIns[7] = "man";
    builtIns[8] = "bash";
    builtIns[9] = "sh";
    builtIns[10] = "nano";
    builtIns[11] = "emacs";
    builtIns[12] = "whoami";
    builtIns[13] = "uname";
    builtIns[14] = "clear";

    //char line[1024];

    char *line; //Pointer to line of text
    char *command[100]; //up to 100 commands
    int count = 0;
    int status;
    //char *in = NULL;
    while(1){ //Loop body here

        //Check for background child processes
        int child = waitpid(-1, &status, WNOHANG);
        if(child>0 && WIFEXITED(status)){
            int exit = WEXITSTATUS(status);
            //printf("Background process %d exited with code %d\n", child, exit);
        }

        printf("%s", prompt);
        //fgets(line, 1023, stdin);
        //scanf("%m[^\n]s", &line);
        count = 0;
        for(int i=0; i<100; i++){
            command[i] = NULL;
        }

        line = getLine();
        char* token;
        
        token = strtok(line, " ");
        while(token != NULL){
            command[count] = token;
            token = strtok(NULL, " ");
            count++;
        }

        if(!command[0]){ //If enter pressed on empty line
            continue;
        }

        if(!strcmp(command[0], builtIns[14])){
            for(int l = 0; l < 100; l++){
                for(int c = 0; c < 100; c++){
                    char randomGarbage = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+~`,<.>?/"[random () % 58];
                    printf("%c", randomGarbage);
                }       
                printf("\n");
            }
            continue;
        }

        if(!strcmp(command[0], builtIns[13])){
            printf(ANSI_COLOR_CYAN  "                   -`                 \n");
            printf("                  .o+`                 \n");
            printf("                 `ooo/                 \n");
            printf("                `+oooo:                \n");
            printf("               `+oooooo:               \n");
            printf("               -+oooooo+:              \n");
            printf("             `/:-:++oooo+:             \n");
            printf("            `/++++/+++++++:            \n");
            printf("           `/++++++++++++++:           \n");
            printf("          `/+++ooooooooooooo/`         \n");
            printf("         ./ooosssso++osssssso+`        \n");
            printf("        .oossssso-````/ossssss+`       \n");
            printf("       -osssssso.      :ssssssso.     \n");
            printf("      :osssssss/        osssso+++.    \n");
            printf("     /ossssssss/        +ssssooo/-    \n");
            printf("   `/ossssso+/:-        -:/+osssso+-  \n");
            printf("  `+sso+:-`                 `.-/+oso: \n");
            printf(" `++:.                           `-/+/\n");
            printf(" .`                                 `/\n");
            printf("\n\t\tOh BTW I use Arch\n\n"    ANSI_COLOR_RESET);
            continue; // Thanks to Screenfetch for the awesome ascii art
        }

        if(!strcmp(command[0], builtIns[11])){
            printf("lol no\n");
            continue;
        }

        if(!strcmp(command[0], builtIns[12])){
            printf("How should I know who you are?\n");
            continue;
        }
        
       if(!strcmp(command[0], builtIns[0])){
            //exit
            printf("Exiting\n");
            break;
        }  
        
        else if(!strcmp(command[0], builtIns[1])){
            //pid does not take args, do nothing if user tries to pass args
            if(command[1]){
                printf("'pid' does not take any arguments.\n");
                continue;
            }
            //print own PID
            printf("%d\n", getpid());
        }

        else if(!strcmp(command[0], builtIns[2])){
            //ppid does not take args, do nothing if user tries to pass args
            if(command[1]){
                printf("'ppid' does not take any arguments.\n");
                continue;
            }
            //print parent PID
            printf("%d\n", getppid());
        }

        else if(!strcmp(command[0], builtIns[3])){
            //Should recieve either one arg or no arguments
            if(command[2]){
                printf("Usage: cd <path>\n");
                continue;
            }
            
            if(command[1]){
                if(chdir(command[1])){
                    perror("Error");
                }
            }
            else{
                if(chdir(getenv("HOME"))){
                    perror("Error");
                }
            }
        }

        else if(!strcmp(command[0], builtIns[4])){
            //print working directory
            //take no args, complain if user tries to pass anything
            if(command[1]){
                printf("'pwd' takes no arguments.\n");
                continue;
            }
            char cwd[1024];
            if(getcwd(cwd, sizeof(cwd)) == NULL){
                printf("Uh-oh.\n");
                continue;
            }
            else{
                printf("%s\n", cwd);
            }
            
        }
        
        else if(!strcmp(command[0], builtIns[5])){
            //If two args, set env variable. If one, clear variable
            if(command[3] || !command[1]){
                printf("Usage: set <var> <value>\n");
                continue;
            }
            
            //if two args recieved
            if(command[2]){ //overwrite with given value
                setenv(command[1], command[2], 1);
            }
            else{ //Overwrite with empty string
                setenv(command[1], "", 1);
            }

        }

        else if(!strcmp(command[0], builtIns[6])){
            //only takes one argument, complain if more or fewer are passed
            if(command[2] || !command[1]){
                printf("Usage: get <var>\n");
                continue;
            }
            //Find if env variable exists
            if(getenv(command[1])){
                printf("%s\n", getenv(command[1]));
            }          
        }

        else if(!strcmp(command[0], builtIns[7])){
            if(command[1]){
                printf("For security purposes, the man page on %s has been removed.\n", command[1]);
            }
            else{
                printf("Fun fact: Google is easier than man.\n");
            }
        }

        else if(!strcmp(command[0], builtIns[8]) || !strcmp(command[0], builtIns[9])){
            printf("No no no, we can't have you getting a better shell that easily.\n");
            printf("There's a much better and safer way to do things.\n");
        } 

        else if(!strcmp(command[0], builtIns[10])){
            printf("Okay, look. I won't stop you from using nano, but you should try vim.\n");
            printf("People say it's intimidating, but it's actually quite simple.\n");
            printf("It's very streamlined and efficient, and the plugins are amazing.\n");
            printf("However, you asked for nano, and you'll get it.\n");
            printf("But first, we're waiting 20 seconds so you can think about using vim.\n");
            for(int c = 20; c > 0; c--){
                printf("."); 
                fflush(stdout);
                sleep(1);
            }
            int ret = fork(); //create fork
            int background = 0; //child in foreground by default
            if(!ret){
                //this is the child process
                //printf("CHILD PID: %d\n", (int) getpid()); //print child pid before executing

                //check to see if should be in background
                if(!strcmp(command[count-1], "&")){ 
                    background = 1; //child will run in background
                    printf("background = %d", background);
                    command[count-1] = NULL; //Otherwise "&" will mess up execvp()
                }
                if(execvp(command[0], command)){
                    perror("Error");
                }
            }
            else{
                //parent process
                if(!background){ //If running in foreground
                    usleep(100000);
                    waitpid(ret, &status, 0);
                    
                    if(WIFEXITED(status)){//Print exit code when child exits
                    //printf("PID %d exited with code %d\n", ret, WIFEXITED(status));
                    }
                }
                else{ 
                    usleep(1000); //not sure why this helps but it does      
                }
                
                
                }
            }

        else{ //Not a builtin command
            int ret = fork(); //create fork
            int background = 0; //child in foreground by default
            if(!ret){
                //this is the child process
                //printf("CHILD PID: %d\n", (int) getpid()); //print child pid before executing

                //check to see if should be in background
                if(!strcmp(command[count-1], "&")){ 
                    background = 1; //child will run in background
                    printf("background = %d", background);
                    command[count-1] = NULL; //Otherwise "&" will mess up execvp()
                }
                if(execvp(command[0], command)){
                    perror("Error");
                }
            }
            else{
                //parent process
                if(!background){ //If running in foreground
                    usleep(100000);
                    waitpid(ret, &status, 0);
                    
                    if(WIFEXITED(status)){//Print exit code when child exits
                    //printf("PID %d exited with code %d\n", ret, WIFEXITED(status));
                    }
                }
                else{ 
                    usleep(1000); //not sure why this helps but it does      
                }
                
                
                }
            }

    }
    return(0);
}

char *getLine(){
    char *line;
    int curChar;
    size_t size = 10;
    size_t length = 0;

    line = realloc(NULL, sizeof(char)*size);
    if(!line){
        return line;
    }

    curChar=fgetc(stdin); //get the first char

    while(curChar != '\n' && curChar != EOF){ //run until newline or file end
        line[length++] = curChar; //copy in char
        if(length == size){ //hit length limit
            line = realloc(line, sizeof(char)*(size+=16)); //realloc more mem
            if(!line){ //return if appropriate
                return line;
            }
        }
        curChar=fgetc(stdin); //next character
    }
    line[length++]='\0'; //null terminator is necessary

    return realloc(line, sizeof(char)*length); //return array of correct length
}
