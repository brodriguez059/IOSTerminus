#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

/////////////////////////////////////// Macros

#define ROOT 0 //The root directory
#define CMDS 1 //The directory where the commands executables lie.
#define SCRT 2 //The directory where the scripts lie.
#define DATA 3 //The directory where the game data lies.
#define GAME 4 //The directory where the game directory lies.
#define DOCS 5 //The directory where the documentation lies.


#define DIRNUM 6

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define error(a) {perror(a); exit(1);};

/////////////////////////////////////// Structs and type definitions

typedef struct {
  char name[512];
  int length;
} dir_t;

/////////////////////////////////////// Global variables

extern dir_t gameDirs[DIRNUM];
