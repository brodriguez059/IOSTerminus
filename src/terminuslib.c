#include "defines.h"

int game_state = E_TUTORIAL;

dir_t gameDirs[NUMDIR];
char welcomePath[512];

void init_dirs(){
   char* root = getcwd(NULL,0);
   int rootLength = strlen(root);

   //Initialize root directory
   strcpy(gameDirs[ROOT].name, root);
   gameDirs[ROOT].length = rootLength;
   
   //Initialize commands directory
   strcpy(gameDirs[CMDS].name, root);
   strcat(gameDirs[CMDS].name, "/bin/commands/");
   gameDirs[CMDS].length = rootLength + strlen("/bin/commands/");

   //Initialize events directory
   strcpy(gameDirs[EVTS].name, root);
   strcat(gameDirs[EVTS].name, "/bin/events/");
   gameDirs[EVTS].length = rootLength + strlen("/bin/events/");

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
   strcat(gameDirs[GAME].name, "/data/Home/");
   gameDirs[GAME].length = rootLength + strlen("/data/Home/");

   //Initialize docs directory
   strcpy(gameDirs[DOCS].name, root);
   strcat(gameDirs[DOCS].name, "/data/docs/");
   gameDirs[DOCS].length = rootLength + strlen("/data/docs/");
}

int execute_cmd(int argc, char *argv[])
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
      //close(p[PIPE_WRITE_END]);

      //dir_t gameDirs[DIRNUM];

      //for(int k = 0; k < DIRNUM; k++){
      //   if(read(p[PIPE_READ_END], &(gameDirs[k]), sizeof(dir_t)) < 0) error("There was a problem reading from the pipe");
      //}

      int plen = gameDirs[CMDS].length + strlen(argv[0]);
      char path[plen];

      strcpy(path, gameDirs[CMDS].name);
      strcat(path, argv[0]);
      //printf("Command to be executed: %s\n", path);

      //close(p[PIPE_READ_END]);
      if(execv(path, argv) < 0){
         error("There was an error during the execution of the generation command");
      }

   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.

      //close(p[PIPE_READ_END]);

      //for(int j = 0; j < 3; j++){
      //   if(write(p[PIPE_WRITE_END], &(gameDirs[j]), sizeof(dir_t)) < 0) error("There was a problem writing to the pipe");
      //}
      
      //close(p[PIPE_WRITE_END]);
      wait(NULL);

   }

   return 0;
}

int execute_ev(int argc, char *argv[])
{
   printf("%d, %s\n", argc, argv[0]);

   return 0;
}

int execute_script(char* path, char* script)
{
   pid_t pid;

   pid = fork();

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");

   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.
      char command[512];

      strcpy(command, path);
      strcat(command, script);

      if(execl("/bin/sh", "sh", "-c", command, (char *) NULL)){
         error("There was an error during the execution of the command");
      }

   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.

      wait(NULL);

   }

   return 0;
}