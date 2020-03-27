// Terminus - myShell0 modification - IOS G4
//////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "cd.h"
#include "defines.h"

#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20

#define ReadEnd 0
#define WriteEnd 1

//Global variables
DirInfo gameDirs[3];


/////////// reading commands:

int read_args(int* argcp, char* args[], int max, int* eofp)
{
   static char cmd[MAXLINE];
   char* cmdp;
   int ret,i;

   *argcp = 0;
   *eofp = 0;
   i=0;
   while ((ret=read(0,cmd+i,1)) == 1) {
      if (cmd[i]=='\n') break;  // correct line
      i++;
      if (i>=MAXLINE) {
         ret=-2;        // line too long
         break;
      }
   }
   switch (ret)
   {
     case 1 : cmd[i+1]='\0';    // correct reading
              break;
     case 0 : *eofp = 1;        // end of file
              return 0;
              break;
     case -1 : *argcp = -1;     // reading failure
              fprintf(stderr, "Reading failure \n");
              return 0;
              break;
     case -2 : *argcp = -1;     // line too long
              fprintf(stderr, "Line too long -- removed command\n");
              return 0;
              break;
   }
   // Analyzing the line
   cmdp= cmd;
   for (i=0; i<max; i++) {  /* to show every argument */
      if ((args[i]= strtok(cmdp, " \t\n")) == (char*)NULL) break;
      cmdp= NULL;
   }
   if (i >= max) {
      fprintf(stderr, "Too many arguments -- removed command\n");
      return 0;
   }
   *argcp= i;
   return 1;
}

///////////////////////////////////////

int execute(int argc, char *argv[])
{
   int p[2];
   if(pipe(p) < 0) error("There was a problem creating the pipe");

   pid_t pid;

   pid = fork();

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");
   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.
      close(p[WriteEnd]);
      DirInfo dirs[3];

      for(int k = 0; k < 3; k++){
         if(read(p[ReadEnd], &(dirs[k]), sizeof(DirInfo)) < 0) error("There was a problem reading from the pipe");
      }

      int plen = dirs[CMDS].length + strlen(argv[0]);
      char path[plen];

      strcpy(path, dirs[CMDS].name);
      strcat(path, argv[0]);
      printf("Command to be executed: %s\n", path);

      close(p[ReadEnd]);
      if(execv(path, argv) < 0){
         error("There was an error during the execution of the command");
      }
   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.

      close(p[ReadEnd]);

      for(int j = 0; j < 3; j++){
         if(write(p[WriteEnd], &(gameDirs[j]), sizeof(DirInfo)) < 0) error("There was a problem writing to the pipe");
      }
      
      close(p[WriteEnd]);
      wait(NULL);
   }

   return 0;
}

void initialize(){
   
   char* root = getcwd(NULL,0);
   int rootLength = strlen(root);

   //Initialize root directory
   strcpy(gameDirs[ROOT].name, root);
   gameDirs[ROOT].length = rootLength;
   
   //Initialize game directory
   strcpy(gameDirs[GAME].name, root);
   strcat(gameDirs[GAME].name, "/data/home/");
   gameDirs[1].length = rootLength + strlen("/data/home/");
   
   //Initialize commands directory
   strcpy(gameDirs[CMDS].name, root);
   strcat(gameDirs[CMDS].name, "/bin/commands/");
   gameDirs[2].length = rootLength + strlen("/bin/commands/");

   /*
   for(int i = 0; i < 3; i++){
      printf("%s : %d \n", gameDirs[i].name, gameDirs[i].length);
   }
   */
}


int t_main ()
{
   initialize();

   char * Prompt = "Terminus> ";
   int eof= 0;
   int argc;
   char *args[MAXARGS];

   while (1) {
      write(0,Prompt, strlen(Prompt));
      if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
         if(strcmp(args[0], "cd") == 0){
            cd(".");
         }else{
            execute(argc, args);
         }
      }
      if (eof) exit(0);
   }
}
