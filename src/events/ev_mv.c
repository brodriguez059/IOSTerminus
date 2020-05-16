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
            write(STDOUT, "HA HA!, I have move the big orc to the big hole and now it is stuck\n", 69);
            write(STDOUT, "By defeating the big orc, I have brought peace to this place and now I can do as I wish.\n", 90);
            write(STDOUT, "'Well done!, my player. You adventure ends here, but you can keep exploring', says a strange voice.", 100);
        } else {
            write(STDOUT,"Oh no!, by moving the orc to another place I have let it escape and now it is heading to the town!\n",100);
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
    case S_RM:;
        if(strcmp("Cliff", argv[2])==0){
            char cave_path[512];

		    strcpy(cave_path, gameDirs[GAME].name);
		    strcat(cave_path, "Forest/Town/Mountain/Cave_entrance");

		    chmod(cave_path, 0777); //To unlock it

            write(STDOUT, "I think I managed to move the boulder to the cliff, now the entrance is clear\n",79);
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
            write(STDOUT,"I think I have moved them to the right place.\n",47);
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
