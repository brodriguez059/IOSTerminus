// Terminus - myShell0 modification - IOS G4
//////////////////////////////////////////////////

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cd.h"
#include "defines.h"

///////////////////////////////////////////////////// Definitions:

/////////////////////////////////////// Macros

#define MAXLINE 200
#define MAXARGS 20

#define ReadEnd 0
#define WriteEnd 1

/////////////////////////////////////// Global Variables
dir_t gameDirs[DIRNUM];
char welcomePath[512];

/////////////////////////////////////// Function signatures
/**
 * This function prints in STDIN a welcoming message and gives some basic
 * information to the player.
 */
int sayWelcome();

/**
 * Before starting the shell, this function is called to generate the game
 * directory and store the paths of the directories that the game uses.
 */
int initialize();

/**
 * This function creates a new process from which the command specified by the
 * user will be executed. If the command is "cd", it will not create another
 * process but will still execute the command.
 */
int execute(char *argv[]);

/**
 * Once the player has decided to finish their session, this function will be
 * called to delete the game directory.
 */
int finalize();

/**
 * This function separates the arguments written in a line and puts them inside
 * a vector of char*
 */
int read_args(int* argcp, char* args[], int max, int* eofp);



///////////////////////////////////////////////////// Implementation

/////////////////////////////////////// Main function. Entry point.

/**
 * Instead of using the typical main, we have had to rename it to "t_main". The
 * reason for this is that it implements the code of cd.c and that code already
 * has a "main". Therefore, instead of having to redesign everything, we just
 * decided to change the entry point of the main executable (which can be done
 * during compilation).
 */
int t_main (){
   char * Prompt = "Terminus> ";
   int eof= 0;
   int argc;
   char *args[MAXARGS];

   initialize();
   sayWelcome();

   while (1) {
      write(0,Prompt, strlen(Prompt));
      if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
         if(strcmp(args[0], "cd") == 0){
            cd(args[1]); //We change directory.
         }else{
            execute(args);
         }
      }
      if (eof){
         finalize();
         break;
      }
   }
   exit(0);
}

/////////////////////////////////////// Auxiliary functions

int sayWelcome(){
   int fd;
   char c;

	fd = open(welcomePath, O_RDONLY);
	
	if(fd < 0) {
		error("There was an error opening the file")
	}

	while(read(fd, &c,sizeof(c)!=0)){
		if(write(STDIN, &c, sizeof(c)) < 0){
			error("There was an error reading the file")
		}
	}
	close(fd);

   return 0;
}

int initialize(){
   
   //We will use "execl" to execute the bash command. We will first fork though.
   //execl("/bin/sh", "sh", "-c", command, (char *) NULL);

   char* root = getcwd(NULL,0);
   int rootLength = strlen(root);

   //Initialize root directory
   strcpy(gameDirs[ROOT].name, root);
   gameDirs[ROOT].length = rootLength;
   
   //Initialize commands directory
   strcpy(gameDirs[CMDS].name, root);
   strcat(gameDirs[CMDS].name, "/bin/commands/");
   gameDirs[CMDS].length = rootLength + strlen("/bin/commands/");

   //Initialize scripts directory
   strcpy(gameDirs[SCRT].name, root);
   strcat(gameDirs[SCRT].name, "/bin/scripts/");
   gameDirs[SCRT].length = rootLength + strlen("/bin/scripts/");

   //Initialize data directory
   strcpy(gameDirs[DATA].name, root);
   strcat(gameDirs[DATA].name, "/data/");
   gameDirs[DATA].length = rootLength + strlen("/data/");

   //Initialize game directory
   strcpy(gameDirs[GAME].name, root);
   strcat(gameDirs[GAME].name, "/data/home/");
   gameDirs[GAME].length = rootLength + strlen("/data/home/");

   //Initialize docs directory
   strcpy(gameDirs[DOCS].name, root);
   strcat(gameDirs[DOCS].name, "/data/docs/");
   gameDirs[DOCS].length = rootLength + strlen("/data/docs/");

   //Initilize the welcome path
   strcpy(welcomePath, gameDirs[DATA].name);
   strcat(welcomePath,"welcome.txt");

   pid_t pid;

   pid = fork();

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");

   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.
      char command[512];
      strcpy(command,gameDirs[SCRT].name);
      strcat(command, "createGameDirectory y");

      if(execl("/bin/sh", "sh", "-c", command, (char *) NULL)){
         error("There was an error during the execution of the command");
      }

   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.

      wait(NULL);

   }

   if(chdir(gameDirs[GAME].name) < 0){
      error("There was an error initializing the game");
   };
   /*
   for(int i = 0; i < DIRNUM; i++){
      printf("%s : %d \n", gameDirs[i].name, gameDirs[i].length);
   }
   */
  return 0;
}

int execute(char *argv[])
{
   //int p[2];
   //if(pipe(p) < 0) error("There was a problem creating the pipe");

   pid_t pid;

   pid = fork();

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");

   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.
      //close(p[WriteEnd]);

      //dir_t gameDirs[DIRNUM];

      //for(int k = 0; k < DIRNUM; k++){
      //   if(read(p[ReadEnd], &(gameDirs[k]), sizeof(dir_t)) < 0) error("There was a problem reading from the pipe");
      //}

      int plen = gameDirs[CMDS].length + strlen(argv[0]);
      char path[plen];

      strcpy(path, gameDirs[CMDS].name);
      strcat(path, argv[0]);
      //printf("Command to be executed: %s\n", path);

      //close(p[ReadEnd]);
      if(execv(path, argv) < 0){
         error("There was an error during the execution of the generation command");
      }

   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.

      //close(p[ReadEnd]);

      //for(int j = 0; j < 3; j++){
      //   if(write(p[WriteEnd], &(gameDirs[j]), sizeof(dir_t)) < 0) error("There was a problem writing to the pipe");
      //}
      
      //close(p[WriteEnd]);
      wait(NULL);

   }

   return 0;
}

int finalize(){
   write(STDOUT,"\nExiting...\n", 12);

   if(chdir(gameDirs[ROOT].name) < 0){
      error("There was an error exiting the game");
   };

   if(execl("/bin/sh", "sh", "-c", "rm -r ./data/home", (char *) NULL)){
      error("There was an error during the execution of the deletion command");
   }

   return 0;
}

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