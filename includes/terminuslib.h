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
 * Those commands can be:
 * cat, cd, concat, cp, grep, ls, man, mv, pwd, rm, touch, whoami
 */
extern int execute_cmd(int argc, char *argv[]);

/**
 * This function prints in STDIN a welcoming message and gives some basic
 * information to the player.
 */
extern int execute_script(char* path, char* script);

/**
 * This function prints in STDIN a welcoming message and gives some basic
 * information to the player.
 */
extern int execute_ev(int argc, char *argv[]);