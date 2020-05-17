#include "terminuslib.h"

#define NKEYS (sizeof(lookuptable)/sizeof(t_mapfunc))

/////////////////////////////////////// Global variables

int game_state;

dir_t gameDirs[NUMDIR];

/////////////////////////////////////// Event functions

/**
 * This will be the event that must be launched when ancient_sign is read.
 */
int ancient_sign(int argc, char* argv[]){
	int res = game_state;
	switch(game_state)
	{
	case S_TUTORIAL:
		;//If we don't add this ; then the compiler refuses to compile this file.
		char forest_path[512];

		strcpy(forest_path, gameDirs[GAME].name);
		strcat(forest_path, "Forest");

		chmod(forest_path, 0777); //To unlock it
		
		res = S_GAME;
	default:
		break;
	}
	return res;
}

/**
 * 
 */
int desk(int argc, char* argv[]){
	int res = game_state;
	switch(game_state)
	{
	case S_MV:
		res = S_RM;
		write(STDOUT, "You use the magic book to decipher this scroll. You have learned the remove ('RM') spell!\n", 91); 
		break;
	case S_RM:
		write(STDOUT, "You have already learned the remove ('RM') spell\n", 50);
	default:
		break;
	}
	return res;
}

/**
 * 
 */
int diary(int argc, char* argv[]){
	int res = game_state;
	// TODO: there is no state for this to be unlocked
	return res;
}

/**
 * 
 */
int spell_book(int argc, char* argv[]){
	int res = game_state;
	switch (game_state)
	{
	case S_MV:
		; //If we don't add this ; then the compiler refuses to compile this file.
		//We unhide the desk
		char forest_path[512];
		char desk_unhidden[512];

		strcpy(forest_path, gameDirs[GAME].name);
		strcat(forest_path, "Forest/Town/Magic_tower/Study_room/");
		strcpy(desk_unhidden, forest_path);
		strcat(forest_path, ".desk");
		strcat(desk_unhidden, "desk");

		rename(forest_path, desk_unhidden);

		//We delete the spell_book
		char* cwd = getcwd(NULL, 0); 
   		char item[512];

   		strcpy(item, cwd);
   		strcat(item,"/");
   		strcat(item, argv[1]);

		unlink(item);

		break;
	default:
		break;
	}
	return res;
}

/**
 * 
 */
int teacher(int argc, char* argv[]){
	int res = game_state;
		write(STDOUT,"\n \n...\n \n",10);
	switch(game_state)
	{
	case S_GAME:
		res = S_MV;
		write(STDOUT,"One hour later, the session finishes and you decide to move on. You have learned about two new spells at least! What were their names?... 'mv' and 'touch'?\n", 157);
		write(STDOUT, "Yes, those were their names. Now you can move items from locations to locations and create lesser version of items\n", 116);
		break;
	case S_MV:
	case S_RM:
		write(STDOUT,"One hour later, the session finishes and you decide to move... on? Wait, the session has been the same as before. You have learnt nothing new.\n", 144);
		break;
	default:
		break;
	}
	return res;
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

    fifo_read(&game_state, gameDirs);

    t_func_event func = keyfromstring(argv[1]);

    if(func != NULL){
        game_state = func(argc, argv);
    }

    return game_state;
}
