#include "defines.h"

/////////////////////////////////////// Global variables

extern dir_t gameDirs[NUMDIR];
extern int game_state;

/////////////////////////////////////// Global functions

/**
 * Before starting the shell, this function is called to generate the game
 * directory and store the paths of the directories that the game uses.
 */
extern int init_dirs();

/**
 * This function executes one of the commands of the shell.
 */
extern int execute_cmd(int argc, char *argv[]);

/**
 * 
 */
extern int execute_script(char* path, char* script);

/**
 * 
 */
extern int execute_ev(int argc, char *argv[]);

/**
 * 
 */ 
extern int fifo_read(int* state, dir_t dirs[]);

/**
 * 
 */ 
extern int fifo_write(int state, dir_t dirs[]);