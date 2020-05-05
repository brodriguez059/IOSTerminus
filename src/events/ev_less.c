#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "defines.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

/////////////////////////////////////// Event functions

/**
 * This will be the event that must be launched when ancient_sign gets moved.
 */
int ancient_sign(int argc, char* argv[]){

}

/**
 * 
 */
int desk(int argc, char* argv[]){

}

/**
 * 
 */
int diary(int argc, char* argv[]){

}

/**
 * 
 */
int spell_book(int argc, char* argv[]){

}

/**
 * 
 */
int teacher(int argc, char* argv[]){

}

/////////////////////////////////////// Main executable

static t_mapfunc lookuptable[] = {
    { "A", ancient_sign }
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
