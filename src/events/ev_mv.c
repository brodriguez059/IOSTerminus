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
    switch (game_state)
    {
    case S_MV:
    case S_RM:
        if(strcmp("Big_hole", argv[2])==0){
            printf("Now the orc is stuck");
        } else {
            printf("Oh no!, the orc has escaped!");
            game_state = S_END;
        }
        break;
    case S_TUTORIAL:
    case S_GAME:
    default:
        break;
    }

    return game_state;
}

/**
 * 
 */
int boulder(int argc, char* argv[]){
    switch (game_state)
    {
    case S_RM:
        if(strcmp("Cliff", argv[2])==0){
            printf("Bye bye rock");
        } else {
            printf("Where you moving it?");
            game_state = S_END;
        }
        break;
    case S_MV:
    case S_TUTORIAL:
    case S_GAME:
    default:
        break;
    }

    return game_state;
}

/**
 * 
 */
int planks(int argc, char* argv[]){
    switch (game_state)
    {
    case S_MV:
        if(strcmp("Small_lake", argv[2])==0){
            printf("Moved to the right place");
        } else {
            printf("Not the right place");
        }
        break;
    case S_TUTORIAL:
    case S_GAME:
    case S_RM:
    default:
        break;
    }

    return game_state;
}
/////////////////////////////////////// Main executable

static t_mapfunc lookuptable[] = {
    { "big_orc", big_orc}, {"boulder", boulder}, {"planks", planks}
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
