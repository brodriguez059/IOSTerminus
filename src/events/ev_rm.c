#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "defines.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

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
    for (i=0; i < NKEYS; i++) {
        t_mapfunc *map = &(lookuptable[i]);
        if (strcmp(map->key, key) == 0)
            return map->ev;
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    t_func_event func = keyfromstring(argv[1]);
    func(argc, argv);
}
