#include "terminuslib.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

/////////////////////////////////////// Global variables

int game_state;

dir_t gameDirs[NUMDIR];

/////////////////////////////////////// Event functions

/**
 *
 */
int boulder(int argc, char* argv[]) {
    char *txt = "The boulder has been converted into small rocks, the path is now clear.\n";
    write(STDOUT, txt, strlen(txt));

    char cave_path[512];
    strcpy(cave_path, gameDirs[GAME].name);
	strcat(cave_path, "Forest/Town/Mountain/Cave_entrance");

	chmod(cave_path, 0777); //To unlock it

    return game_state;
}

/**
 *
 */
int spirit(int argc, char* argv[]) {
    return game_state;
}

/**
 *
 */
int torch(int argc, char* argv[]) {
    return game_state;
}

int killNpc(int argc, char* argv[]) {
    char *txt;
    char *item = argv[1];

    if (strcmp(item, "foul_goblin") == 0) {
        txt = "You have vaporized the foul goblin.\n";
        //We have to unlock the other cave
        char caveeast_path[512];

        strcpy(caveeast_path, gameDirs[GAME].name);
        strcat(caveeast_path, "Forest/Town/Mountain/Cave_entrance/Cave(east)");
        chmod(caveeast_path, 0777);

    } else if (strcmp(item, "big_orc") == 0) {
        txt = "Looks like I zombified the orc!\n";
        //Create a new big_orc but with another description.
        creat("big_orc", 0666);
    } else {
        txt = "You have killed a inocent character.\nYou regret it too much and execute a spell to go back into a time where you did not have any magic powers.\n";
        game_state = S_END;
    }

    write(STDOUT, txt, strlen(txt));

    return game_state;
}

/////////////////////////////////////// Main executable

static t_mapfunc lookuptable[] = {
    { "foul_goblin", killNpc },
    { "big_orc", killNpc },
    { "hidi", killNpc },
    { "hronar", killNpc },
    { "siraye_arazana", killNpc },
    { "zemra", killNpc },
    { "secretary", killNpc },
    { "teacher", killNpc },
    { "boulder", boulder},
    { "spirit", spirit},
    { "torch", torch}
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
    int result = game_state;

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

    switch (game_state) {
        case S_RM:
            ;//C doesn't let us declare variables after a label.
            //printf("We have entered to the case state: %d\n",game_state);
            if(func != NULL){
                result = func(argc, argv);
            }
            break;
        default:
            //printf("We have entered to the case state: %d\n",game_state);
            ;//C doesn't let us declare variables after a label.
            char *txt = "You have not obtained this spell yet.\n";
            write(STDOUT, txt, strlen(txt));
            break;
    }

    return result;
}
