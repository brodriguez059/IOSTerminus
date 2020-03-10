// Terminus - myShell0 modification - IOS G4
//////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20

static const char* mainPath = NULL;
static int mainPathLength;
static const char* cmdPath = NULL;
static int cmdPathLength;

static const char* gamePath = NULL;
static int gamePathLength;

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
              fprintf(stderr,"Reading failure \n");
              return 0;
              break;
     case -2 : *argcp = -1;     // line too long
              fprintf(stderr,"Line too long -- removed command\n");
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
      fprintf(stderr,"Too many arguments -- removed command\n");
      return 0;
   }
   *argcp= i;
   return 1;
}

///////////////////////////////////////

int execute(int argc, char *argv[])
{
   int pid;

   printf("%s + %d| %s + %d\n",cmdPath,cmdPathLength,argv[0],strlen(argv[0]));

   pid = fork();

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");
   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.
      chdir(gamePath);

      printf("%s + %d| %s + %d\n",cmdPath,cmdPathLength,argv[0],strlen(argv[0]));
      //Main shell always in IOSTerminus/, when entering in child chdir() to current gameDirectory.
      char path[cmdPathLength+((long unsigned int) strlen(argv[0]))];
      strcpy(path,cmdPath);
      strcat(path,argv[0]);

      //This works for default system commands. Doesn't work for user defined commands.
      if(execv(path, argv) < 0){
      //if(execvp(argv[0], argv) < 0){
         error("There was an error during the execution of the command");
      }
   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.
      int* wstatus = NULL;
      int option = 0;
      //TODO add error handling
      waitpid(pid, wstatus, option);
   }
   chdir(mainPath);
   return 0;
}

int initializePaths(){
   mainPath = getcwd(NULL,0);
   mainPathLength = strlen(mainPath);
   printf("%s + %d\n",mainPath,mainPathLength);

   char cmdTemp[mainPathLength + 14];
   cmdPathLength = mainPathLength + 14;
   strcpy(cmdTemp, mainPath);
   strcat(cmdTemp,"/bin/commands/");
   cmdPath = cmdTemp;
   printf("%s + %d\n",cmdPath,cmdPathLength);

   char gameTmp[mainPathLength+11];
   gamePathLength = mainPathLength + 11;
   strcpy(gameTmp, mainPath);
   strcat(gameTmp,"/data/home/");
   gamePath = gameTmp;
   printf("%s + %d\n",gamePath,gamePathLength);
   return 0;
}


int main ()
{
   initializePaths();

   char * Prompt = "myShell0> ";
   int eof= 0;
   int argc;
   char *args[MAXARGS];

   while (1) {
      write(0,Prompt, strlen(Prompt));
      if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
         execute(argc, args);
      }
      if (eof) exit(0);
   }
}

