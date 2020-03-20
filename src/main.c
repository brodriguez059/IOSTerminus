#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include "defines.h"

#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20

//Global variables
char* mainDir;
char gameDir[1024];
char cmdDir[1024];

char* getCmdDir(){
   return NULL;
}

void initialize(){
   mainDir = getcwd(NULL,0);

   strcpy(cmdDir, mainDir);
   strcat(cmdDir,"/bin/commands/");

   strcpy(gameDir, mainDir);
   strcat(gameDir, "/data/home/");
}

int main()
{
initialize();

   int pid;

   pid = fork();

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");
   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.
      char* const* argv = NULL;
      if(execv("./bin/commands/shell", argv) < 0){
         error("There was an error during the execution of the command");
      }
   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.
      int* wstatus = NULL;

      //TODO add error handling
      waitpid(pid, wstatus, 0);
   }

   return 0;
}