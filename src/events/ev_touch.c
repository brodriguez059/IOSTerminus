#include "terminuslib.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

/////////////////////////////////////// Global variables

int game_state;

dir_t gameDirs[NUMDIR];

/////////////////////////////////////// Event functions

/**
 *
 */
int boulder(int argc, char* argv[]){
    char *txt = "The boulder has been caught. \n"
    write(STDOUT, txt, strlen(txt));
    return game_state;
}

/**
 * 
 */
int torch(int argc, char* argv[]){
    char *txt = "Nice torch, now I can see some roads. \n"
    write(STDOUT, txt, strlen(txt));
    return game_state;
}

/**
 * 
 */
int NewCabes(int argc, char* argv[]){
    
        char cavewest_path[512];

        strcpy(cavewest_path, gameDirs[GAME].name);
        strcat(cavewest_path, "Forest/Town/Mountain/Caveentrance/Cave(west)");
        

        char caveeast_path[512];

        strcpy(caveeast_path, gameDirs[GAME].name);
        strcat(caveeast_path, "Forest/Town/Mountain/Caveentrance/Cave(east)");    

}

/////////////////////////////////////// Main executable

static t_mapfunc lookuptable[] = {
    { "torch", torch }
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
    fifo_read(&game_state, gameDirs);

    t_func_event func = keyfromstring(argv[1]);
    int result = func(argc, argv);

    exit(result);
}
