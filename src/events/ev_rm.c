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
    char *item = argv[0];

    if (strcmp(item, "foul_globin") == 0) {
        txt = "You have vaporized the foul goblin.\n";

    } else if (strcmp(item, "big_orc") == 0) {
        txt = "This orc seems inmune to my magic.\n";

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
    { "teacher", killNpc }
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

    switch (game_state) {
        case S_RM:
            ;//C doesn't let us declare variables after a label.
            result = func(argc, argv);
            break;
        default:
            ;//C doesn't let us declare variables after a label.
            char *txt = "You have not obtained this spell yet.\n";
            write(STDOUT, txt, strlen(txt));
            break;
    }

    return game_state;
}
