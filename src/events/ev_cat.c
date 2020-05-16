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
    //I suppose concatenating the big orc with another thing should count as killing it.
    int res = game_state;

    return res;
}

/**
 * 
 */
int foul_goblin(int argc, char* argv[]){
    //I suppose concatenating another thing with the foul goblin could count as killing it.
    int res = game_state;

    return res;
}

/**
 * This handles the events for all sentient creatures. (Hidi, Hronar, Secretary, Siraye_Arazana, Teacher, Zemra)
 */
int sentient(int argc, char* argv[]){
    int res = game_state;
    switch (game_state)
    {
    default:
        //Using the cat on another sentient creature causes a game over.
        write(STDOUT,"'Oh no... what have you done?', says a strange voice as you concatenate two sentient creatures into one\n",105);
        write(STDOUT,"\n It appears that you have commited a sin against nature itself. The guards of the town capture you and pull you into a cell. Game over.",137);
        res = S_END;
        break;
    }
    return res;
}

/**
 * This will get called by either using cat on the planks and the broken boat.
 */
int repair_boat(int argc, char* argv[]){
    int res = game_state;
    //First check that the arguments are correct:
    int boat = ( ( strcmp("broken_boat", argv[1]) == 0 ) || ( strcmp("broken_boat", argv[2]) == 0) );
    int planks = ( ( strcmp("planks", argv[1]) == 0 ) || ( strcmp("planks", argv[2]) == 0 ) );
    if(boat && planks){
        switch (game_state)
        {
        case S_MV:
            //You managed to repair the broken boat, then:

            //We modify the description file of the small lake.
            write(STDOUT,"You can go now to the small island!\n",37);

            //We unlock the small island.
            char island_path[512];

		    strcpy(island_path, gameDirs[GAME].name);
		    strcat(island_path, "Forest/Small_lake/Small_island");

		    chmod(island_path, 0777); //To unlock it
            break;
        default:
            break;
        }
    }else{
        //You concatenated the planks or the broken boat with other things, this stops the player from actually progressing. Game over.
        res = S_END;
    }
    return res;
}

t_mapfunc lookuptable[] = {
    {"big_orc", big_orc}, {"broken_boat", repair_boat}, {"foul_globlin", foul_goblin}, {"planks", repair_boat},
    {"hidi", sentient}, {"hronar", sentient}, {"secretary", sentient}, {"siraye_arazana", sentient}, {"teacher", sentient},
    {"zemra", sentient}
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
