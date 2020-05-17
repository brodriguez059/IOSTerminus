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
    write(STDOUT, "Seems like fusing the big orc with something else is a stronger spell that just destroying it. Why did you know this spell since the beggining?, looks to overpowered.\n",168);
    write(STDOUT, "Well, it doesn't matter. The orc is now a paste of morfed things, an amalgamation, indeed.\n",92);
    write(STDOUT, "By defeating the big orc, I have brought peace to this place and now I can do as I wish.\n", 90);
    write(STDOUT, "'Well done!, my player. You adventure ends here, but you can keep exploring', says a strange voice.", 100);
    return res;
}

/**
 * 
 */
int foul_goblin(int argc, char* argv[]){
    //I suppose concatenating another thing with the foul goblin could count as killing it.
    int res = game_state;
    write(STDOUT, "You have morfed the foul goblin into something that can do no harm. Though, I would not approach it if I were you, it looks a bit... bad.\n",139);
    //We have to unlock the other cave
    char caveeast_path[512];

    strcpy(caveeast_path, gameDirs[GAME].name);
    strcat(caveeast_path, "Forest/Town/Mountain/Cave_entrance/Cave(east)");
    chmod(caveeast_path, 0777);
    return res;
}

/**
 * This handles the events for all sentient creatures. (Hidi, Hronar, Secretary, Siraye_Arazana, Teacher, Zemra)
 */
int sentient(int argc, char* argv[]){
    int res = game_state;
    //Using the cat on another sentient creature causes a game over.
    write(STDOUT,"'Oh no... what have you done?', says a strange voice as you concatenate two sentient creatures into one\n",105);
    write(STDOUT,"\n It appears that you have commited a sin against nature itself. The guards of the town capture you and pull you into a cell. Game over.",137);
    res = S_END;
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
            write(STDOUT,"Using the spell and the planks that you found, you manage to repair the boat.\n",79);
            write(STDOUT,"'Great!, now I can go to the small island', you say\n",53);

            //We modify the description file of the small lake.
            int fd_lake;
            if((fd_lake = open(".description", O_WRONLY)) < 0){
                error("There were magical problems");
            }
            int length;
            if((length = lseek(fd_lake, 0, SEEK_END)) < 0){
                error("There were magical errors changing the cursors");
            }
            char *text = "Finally!, the boat is repaired and now I can go to the small island\n\n";
            int n = strlen(text);
            int i;
            char c;
            for(i = 0; i < n; i++){
                c = text[i];
                if(write(fd_lake, &c, 1) < 0){
                    error("There were some fatal magical problems");
                }
            }
            close(fd_lake);

            //We unlock the small island.
            char island_path[512];

		    strcpy(island_path, gameDirs[GAME].name);
            strcat(island_path,"Forest/Small_lake/");

            char concatenated_boat[512];
            strcpy(concatenated_boat, island_path);
            if( strcmp("planks", argv[1]) == 0 ){
                strcat(concatenated_boat,"planks_broken_boat");
            }else{
                strcat(concatenated_boat,"broken_boat_planks");
            }

            unlink(concatenated_boat); //We delete the repaired boat.
            
		    strcat(island_path, "Small_island");

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
    {"Hidi", sentient}, {"Hronar", sentient}, {"secretary", sentient}, {"Siraye_Arazana", sentient}, {"teacher", sentient},
    {"Zemra", sentient}
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
    // printf("(Event) My current state is: %d\n",game_state);
    // int j;
    // for(j=0;j<NUMDIR;j++){
    //     printf("gameDirs[%d]: %s\n",j,gameDirs[j].name);
    // }

    // printf("We are going to launch a mv event\n");
    // printf("My arguments are:\n");

    if(func != NULL){
        game_state = func(argc, argv);
    }

    return game_state;
}
