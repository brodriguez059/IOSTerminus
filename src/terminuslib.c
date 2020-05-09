#include "terminuslib.h"

int game_state = S_TUTORIAL;

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
         error("There was an error during the execution of the generation command");
      }

   }else{
      //If fork returned a positive number, then we are in the parent process
      //and pid is the process id of the child.
      int b = strcmp(argv[0], "man");
      int num;
      if(b == 0){
         num = 2 + NUMDIR;
         fifo_write(num, game_state, gameDirs);
      }else{
         wait(NULL);
      }

   }

   return 0;
}

int execute_ev(int argc, char *argv[])
{
   pid_t pid;

   pid = fork();

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

      //close(p[PIPE_READ_END]);
      if(execv(path, argv) < 0){
         error("There was an error during the execution of the generation command");
      }

   }else{
      int num_info = 2;
      fifo_write(num_info, game_state, gameDirs);

   }
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

int fifo_read(int* argc, int* state, dir_t dirs[]){
   sleep(1); //We need to give the parent some time.
   const char* file = "./fifoChannel";
	int fd = open(file, O_RDONLY);
	if (fd < 0) return -1; /* no point in continuing */

   read(fd, argc, sizeof(int));
   read(fd, state, sizeof(int));

   int MaxLoops = *argc - 2;

   int n;

   int i;
   for (i = 0; i < MaxLoops; i++) {
      n = read(fd, &(dirs[i]), sizeof(dir_t));
      printf("Read: %s %d\n", gameDirs[i].name, gameDirs[i].length);
      if (n == 0) break;  
   }

	close(fd);       /* close pipe from read end */
	unlink(file);    /* unlink from the underlying file */

	return 0;
}

int fifo_write(int argc, int state, dir_t dirs[]){
	const char* pipeName = "./fifoChannel";
	mkfifo(pipeName, 0666);                      /* read/write for user/group/others */
	int fd = open(pipeName, O_CREAT | O_WRONLY); /* open as write-only */
	if (fd < 0) return -1;                       /* can't go on */

   printf("Writing: %d\n", argc);
   write(fd, &argc, sizeof(int));
   printf("Writing: %d\n", state);
   write(fd, &state, sizeof(int));

   int MaxLoops = argc - 2;

	int i;
	for (i = 0; i < MaxLoops; i++) {
      printf("Writing: %s %d\n", gameDirs[i].name, gameDirs[i].length);
      write(fd, &(dirs[i]), sizeof(dir_t));
   }
	close(fd);           /* close pipe: generates an end-of-stream marker */

   wait(NULL); /*We must first wait until all the info has been read */

	unlink(pipeName);    /* unlink from the implementing file */

	return 0;
}