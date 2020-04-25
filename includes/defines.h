#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/////////////////////////////////////// Macros

#define NUMDIR 7

#define ROOT 0 //The root directory
#define CMDS 1 //The directory where the commands executables are.
#define EVTS 2 //The directory where the events executables are.
#define SCRT 3 //The directory where the scripts are.
#define DATA 4 //The directory where the game data is.
#define GAME 5 //The directory where the game directory is.
#define DOCS 6 //The directory where the documentation is.

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

#define E_TUTORIAL 0
#define E_GAME 1
#define E_MV 2
#define E_RM 3
#define E_END 4

#define error(a) {perror(a); exit(1);};

/////////////////////////////////////// Structs and type definitions

typedef struct {
  char name[512];
  int length;
} dir_t;

/*
switch(game_state){
               case E_TUTORIAL:
                  if( (strcmp(args[0], "mv") == 0) || (strcmp(args[0],"rm") == 0) ){
                     int n;
                     n = write(STDOUT, "Hmmm...?, what are you trying to do?, maybe you should learn", 61);
                     if(n<0) error("There was an error writing on state E_TUTORIAL");
                     n = write(STDOUT, " how to use that spell first.\n", 30);
                     if(n<0) error("There was an error writing on state E_TUTORIAL");
                  }
                  break;
               case E_GAME:
                  break;
               case E_MV:
                  break;
               case E_RM:
                  break;
               case E_END:
               default:
                  break;
               }
*/