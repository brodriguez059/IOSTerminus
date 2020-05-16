#include "terminuslib.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

/////////////////////////////////////// Global variables

int game_state;

dir_t gameDirs[NUMDIR];

bool spell_book_found = 0;

/////////////////////////////////////// Event functions

/**
 * This will be the event that must be launched when ancient_sign is read.
 */
int ancient_sign(int argc, char* argv[]){
	switch(game_state)
	{
	case S_TUTORIAL:
		game_state = S_GAME;
	default:
		break;
	}
}

/**
 * 
 */
int desk(int argc, char* argv[]){
	switch(game_state)
	{
	case S_MV:
		if(spell_book_found == 1)
		{
			game_state = S_RM;
			write(1, "You use the magic book to decipher this scroll. You have learned the remove ('RM') spell!\n", 91); 
		} else
		{
			write(1, "You have to find a way to decipher this scroll\n", 48);
		}
		break;
	case S_GAME:
		write(1, "You should find someone who could teach you magic first.\n", 57); 
	default:
		break;
	}
}

/**
 * 
 */
int diary(int argc, char* argv[]){
	// TODO: there is no state for this to be unlocked
}

/**
 * 
 */
int spell_book(int argc, char* argv[]){
	spell_book_found = 1;
}

/**
 * 
 */
int teacher(int argc, char* argv[]){
	switch(game_state)
	{
	case S_GAME:
		game_state = S_MV;
	default:
		break;
	}
}

/////////////////////////////////////// Main executable

static t_mapfunc lookuptable[] = {
    { "ancient_sign", ancient_sign }, {"desk", desk}, {"diary", diary}, {"spell_book", spell_book}, {"teacher", teacher}
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
