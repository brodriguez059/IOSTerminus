#include "terminuslib.h"

int game_state;

dir_t gameDirs[NUMDIR];
char welcomePath[512];

int init_dirs(){
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

   return 0;
}

int execute_cmd(int argc, char *argv[])
{
   pid_t pid;

   pid = fork();

   int return_code = 0;

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");

   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.
      int plen = gameDirs[CMDS].length + strlen(argv[0]);
      char path[plen];

      strcpy(path, gameDirs[CMDS].name);
      strcat(path, argv[0]);
      //printf("Command to be executed: %s\n", path);

      if(execv(path, argv) < 0){
         error("There was an error during the execution of the command");
      }

   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.

      int b = strcmp(argv[0], "man"); // The command "man" requires info from a FIFO, that's why we do this comprobation.

      if(b == 0){
         return_code = fifo_write(game_state, gameDirs);
         //We don't use wait here but rather inside the function fifo_write as it
         //needs to know it the other end of the FIFO has already finished reading
      }else{
         int wstate;

         wait(&wstate);

         if (WIFEXITED(wstate)){
            return_code = WEXITSTATUS(wstate);
            //printf("exit status = %d\n", return_code);
         }else{
            return_code = 1;
         }
      }

      //printf("We will return: %d\n",return_code);
   }
   return return_code;
}

int execute_ev(int argc, char *argv[])
{
   pid_t pid;

   pid = fork();

   int return_code = game_state;

   if(pid < 0){
      //If fork returned a negative number, there was an error.
      error("There was an error during the fork");

   }else if(pid == 0){
      //If fork returned 0, then we are in the child process.

      int plen = gameDirs[EVTS].length + 3 + strlen(argv[0]);
      char path[plen];

      strcpy(path, gameDirs[EVTS].name);
      strcat(path, "ev_");
      strcat(path, argv[0]);
      //printf("Command to be executed: %s\n", path);

      if(execv(path, argv) < 0){
         error("There was an error during the execution of the event");
      }

   }else{
      //We will always use FIFOs for the events.
      return_code = fifo_write(game_state, gameDirs);
   }
   return return_code;
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
         error("There was an error during the execution of the script");
      }
   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.
      wait(NULL);
   }

   return 0;
}

int fifo_read(int* state, dir_t dirs[]){
   sleep(1); //We need to give the parent some time.
   const char* file = "./fifoChannel";
	int fd = open(file, O_RDONLY);
	if (fd < 0) return -1; /* no point in continuing */

   read(fd, state, sizeof(int));

   int n;

   int i;
   for (i = 0; i < NUMDIR; i++) {
      n = read(fd, &(dirs[i]), sizeof(dir_t));
      //printf("Read: %s %d\n", gameDirs[i].name, gameDirs[i].length);
      if (n == 0) break;  
   }

	close(fd);       /* close pipe from read end */
	unlink(file);    /* unlink from the underlying file */

	return 0;
}

int fifo_write(int state, dir_t dirs[]){
	const char* pipeName = "./fifoChannel";
   mknod(pipeName, S_IFIFO | 0666, 0); /* read/write for user/group/others */
	//mkfifo(pipeName, 0666);
   int fd = open(pipeName, O_CREAT | O_WRONLY); /* open as write-only */
	if (fd < 0) return -1;                       /* can't go on */

   //printf("Writing: %d\n", state);
   write(fd, &state, sizeof(int));

	int i;
	for (i = 0; i < NUMDIR; i++) {
      //printf("Writing: %s %d\n", gameDirs[i].name, gameDirs[i].length);
      write(fd, &(dirs[i]), sizeof(dir_t));
   }
	close(fd);           /* close pipe: generates an end-of-stream marker */

   int wstate;
   
   wait(&wstate); /*We must first wait until all the info has been read */
   int return_code;
   if (WIFEXITED(wstate)){
      return_code = WEXITSTATUS(wstate);
   }else{
      return_code = 1;
   }

	unlink(pipeName);    /* unlink from the implementing file */

	return return_code;
}