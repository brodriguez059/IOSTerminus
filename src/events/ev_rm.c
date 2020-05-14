#include "terminuslib.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

/////////////////////////////////////// Global variables

int game_state;

dir_t gameDirs[NUMDIR];

/////////////////////////////////////// Event functions

/**
 * 
 */
int big_orc(int argc, char* argv[]){

}

/**
 * 
 */
int boulder(int argc, char* argv[]){

}

/**
 * 
 */
int foul_goblin(int argc, char* argv[]){

}

/**
 * 
 */
int hidi(int argc, char* argv[]){

}

/**
 * 
 */
int hronar(int argc, char* argv[]){

}
/**
 * 
 */
int secretary(int argc, char* argv[]){

}

/**
 * 
 */
int siraye_arazana(int argc, char* argv[]){

}

/**
 * 
 */
int spirit(int argc, char* argv[]){

}

/**
 * 
 */
int teacher(int argc, char* argv[]){

}

/**
 * 
 */
int torch(int argc, char* argv[]){

}

/**
 * 
 */
int zemra(int argc, char* argv[]){

}

/////////////////////////////////////// Main executable

static t_mapfunc lookuptable[] = {
    { "big_orc", big_orc }
};

t_func_event keyfromstring(char *key)
{
    int i;
    for (i=0; (long unsigned int) i < NKEYS; i++) {
        t_mapfunc *map = &(lookuptable[i]);
        if (strcmp(map->key, key) == 0)
            return map->ev;
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    int nArgFifo;
    fifo_read(&nArgFifo, &game_state, gameDirs);

    t_func_event func = keyfromstring(argv[1]);
    int result = func(argc, argv);

    exit(result);
}
