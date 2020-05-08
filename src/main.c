// Terminus - myShell0 modification - IOS G4
//////////////////////////////////////////////////

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "built-ins.h"
#include "terminuslib.h"

///////////////////////////////////////////////////// Definitions:

/////////////////////////////////////// Macros

#define MAXLINE 200
#define MAXARGS 20

/////////////////////////////////////// Global Variables
int game_state;

/////////////////////////////////////// Function signatures
/**
 * This function prints in STDIN a welcoming message and gives some basic
 * information to the player.
 */
int sayWelcome();

/**
 * Before starting the shell, this function is called to generate the game
 * directory and store the paths of the directories that the game uses.
 * We also need to put the player in the game directory.
 */
int initialize();

/**
 * This function creates a new process from which the command specified by the
 * user will be executed. If the command is "cd", it will not create another
 * process but will still execute the command.
 */
int execute(int argc, char *argv[]);

/**
 * Once the player has decided to finish their session, this function will be
 * called to delete the game directory.
 */
int finalize();

/**
 * Checks whether the command written by the player depends on the current state
 * of the game or not.
 */
int is_state_dependend(char* command);

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
   
   while (1) {
      write(0,Prompt, strlen(Prompt));
      if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
         //We first check for the state-dependend commands.
         if(is_state_dependend(args[0])){
            //We need to specify the path to the event directory and the current state.
            execute(argc, args);
         }else{
            //We need to check if man is the command to be executed to prepare the doc directory.
            if(strcmp(args[0],"man") == 0){
               execute(argc, args);
            }else{
               //We don't need to keep checking, the command doesn't depend on anything initialized in the main.
               execute(argc, args);
            }
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

   //We do this because sayWelcome() executes before initiliaze()
   //and this last function is the one which puts the player in
   //the game directory. When this gets executed, we are still
   //in the project root directory.
	fd = open("./data/welcome.txt", O_RDONLY);
	
	if(fd < 0) {
		error("There was an error opening the file")
	}

	while(read(fd, &c, sizeof(c)) > 0){
		if(write(STDIN, &c, sizeof(c)) < 0){
			error("There was an error reading the file")
		}
	}
	close(fd);

   return 0;
}

int initialize()
{
   init_dirs();
   execute_script(gameDirs[SCRT].name, "createGameDirectory y");
   sayWelcome();

   if(chdir(gameDirs[GAME].name) < 0){
      error("There was an error initializing the game");
   };
   //For primite debugging.
   //   int i;
   //   for(i = 0; i<NUMDIR; i++ ){
   //      printf("Directory: %s | %d \n", gameDirs[i].name, gameDirs[i].length);
   //   }

  return 0;
}

int execute(int argc, char *argv[])
{
   if(strcmp(argv[0], "cd") == 0){
      //We change directory. This function is a built-in of the 
      //shell, so we don't call execute_cmd() to fork and exec.
      cd(argv[1]); 
   }else if((strcmp(argv[0], "exit")) == 0 || (strcmp(argv[0], "logout") == 0)){
      finalize();
      logout();
   }else{
      execute_cmd(argc, argv);
   }

   return 0;
}

int finalize()
{
   write(STDOUT,"\nExiting...\n", 12);

   if(chdir(gameDirs[DATA].name) < 0){
      error("There was an error exiting the game");
   };

   char deleteCommand[512];
   strcpy(deleteCommand, "deleteGameDirectory ");
   strcat(deleteCommand, gameDirs[GAME].name);
   execute_script(gameDirs[SCRT].name, deleteCommand);

   return 0;
}

int is_state_dependend(char* command){
   char* cmds[5] = {"cat","less","mv","rm","touch"};
   	int i;
   	for(i=0;i<4;i++){
    	   if(strcmp(command, cmds[i]) == 0){
           	return 1;
         }
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