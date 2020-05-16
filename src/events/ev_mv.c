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
    int res = game_state;
    switch (game_state)
    {
    case S_RM:
        if(strcmp("Big_hole", argv[2])==0){
            printf("Now the orc is stuck");
        } else {
            printf("Oh no!, the orc has escaped!");
            res = S_END;
        }
        break;
    default:
        break;
    }

    return res;
}

/**
 * 
 */
int boulder(int argc, char* argv[]){
    int res = game_state;
    switch (game_state)
    {
    case S_RM:
        if(strcmp("Cliff", argv[2])==0){
            printf("Bye bye rock");
        } else {
            printf("Where you moving it?");
            res = S_END;
        }
        break;
    default:
        break;
    }

    return res;
}

/**
 * 
 */
int planks(int argc, char* argv[]){
    int res = game_state;
    switch (game_state)
    {
    case S_MV:
        if(strcmp("Small_lake", argv[2])==0){
            printf("Moved to the right place");
        } else {
            printf("Not the right place");
        }
        break;
    default:
        break;
    }

    return res;
}
// /////////////////////////////////////// Main executable

t_mapfunc lookuptable[] = {
    { "big_orc", big_orc}, {"boulder", boulder}, {"planks", planks}
};

t_func_event keyfromstring(char *key)
{
    //printf("The key is: %s\n", key);

    int i;
    for (i=0; (long unsigned int) i < NKEYS; i++) {
        t_mapfunc *map = &(lookuptable[i]);

        //printf("We will compare %s with %s\n", key, map->key);

        if (strcmp(map->key, key) == 0)
            return map->ev;
    }
    return NULL;
}

/**
 * This is the first event file that we created to test everything, that's why there is a lot of commented code.
 */
int main(int argc, char* argv[])
{
    // printf("We are going to launch a mv event, we start reading\n");

    fifo_read(&game_state, gameDirs);

    // printf("(Event) My current state is: %d\n",game_state);
    // int j;
    // for(j=0;j<NUMDIR;j++){
    //     printf("gameDirs[%d]: %s\n",j,gameDirs[j].name);
    // }

    // printf("We are going to launch a mv event\n");
    // printf("My arguments are:\n");
    // int i;
    // for(i = 0; i < argc; i++){
    //     printf("%s\n", argv[i]);
    // }

    t_func_event func = keyfromstring(argv[1]);

    if(func != NULL){
        game_state = func(argc, argv);
    }

    return game_state;
}
