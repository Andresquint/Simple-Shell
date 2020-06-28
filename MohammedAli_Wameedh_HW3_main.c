/**************************************************************
* Class:  CSC-415-02 Summer 2020
* Name: Wameedh Mohammed Ali
* Student ID: 920678405
* Project: C-program, Assignment 3 - Simple Shell
*
* File: MohammedAli_Wameedh_HW3_main
*
* Description: simple shell program that reads lines from user input, then parse and execute the commands by forking/creating new processes.
*
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define BUFFERSIZE 1024	//command line should not be more than 1024

extern int errno ;

void readCmd(char cmd[], char * par[]){
    // Saves user input  it into command line
    if(fgets(cmd, BUFFERSIZE, stdin)) {

        size_t length = strlen(cmd); // Size of the the read/input string
        if (cmd[length - 1] == '\n') {
            cmd[length - 1] = '\0'; // if end of line then null terminate
        }
        char *token;
        token = strtok(cmd, " "); // tokenize the string using space delimiter
        int i = 0;
        while (token != NULL) {
            par[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        par[i] = NULL;
    }else {
    // End of file
        exit(0);
    }
}

int main(int argc, char* argv[]){

    char* path = "/bin/"; // The path of the environment variable for execvp()
    char fullPath[20];
    char* args[100];
    char commandLine[BUFFERSIZE]; //whole command line that the user inputs


    while(1) { // Infinet loop until the user type "exit"
        //will take in user input
        printf("prompt$ ");
        readCmd(commandLine,args);
        // if the input "exit" then break the loop
        if( strcmp(commandLine, "exit") == 0)
        {
            break;
        }

        pid_t childProcessID; // Child ID
        int stat; // Exit status

        if (fork() == 0){ // fork to child process
            //Child
            strcpy(fullPath, path); // copy "/bin/" to full path
            strcat(fullPath, args[0]); // concatenate the full path with command that being requested
            if(execvp(fullPath, args) < 0){ // if execvp fails
                printf("Invalid input failed. Exit with error %d\n", errno);
                exit(2);
            }

        } else {
            //wait for parent
            childProcessID = wait(&stat);
            if (WIFEXITED(stat)) {
                // print child ID and return status
            printf("Child %d, exited with %d\n", childProcessID, WEXITSTATUS(stat));
            }
        }
    }
    return 0;
}