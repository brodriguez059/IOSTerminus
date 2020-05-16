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
int broken_boat(int argc, char* argv[]){

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
int planks(int argc, char* argv[]){
    
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
int teacher(int argc, char* argv[]){

}

/**
 * 
 */
int zemra(int argc, char* argv[]){

}


t_mapfunc lookuptable[] = {
    
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
    fifo_read(&game_state, gameDirs);

    t_func_event func = keyfromstring(argv[1]);

    if(func != NULL){
        game_state = func(argc, argv);
    }

    return game_state;
}
