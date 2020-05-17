#include "terminuslib.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

/////////////////////////////////////// Global variables

int game_state;

dir_t gameDirs[NUMDIR];

/////////////////////////////////////// Event functions

/**
 * 
 */
int torch(int argc, char* argv[]){
    char *txt = "Nice torch, now I can see some roads. \n";
    write(STDOUT, txt, strlen(txt));

    char cavewest_path[512];

    strcpy(cavewest_path, gameDirs[GAME].name);
    strcat(cavewest_path, "Forest/Town/Mountain/Cave_entrance/Cave(west)");
    chmod(cavewest_path, 0777);

    return game_state;
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
    int result = func(argc, argv);

    exit(result);
}
