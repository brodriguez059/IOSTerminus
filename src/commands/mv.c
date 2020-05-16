#include "defines.h"

int main(int argc, char const *argv[])
{
    /* Check for insufficient parameters */

    if (argc < 3) {
       write(STDOUT, "Hmm... you should try to use more arguments, yes?\n", 51);
       exit(1);
    }

    //First we copy the file to the directory
    char buffer[512];
    int srcF;
    int destF;

    char* cpath = getcwd(NULL, 0);

    int count;

    if((srcF = open(argv[1], O_RDWR)) < 0) error("There was an error opening the file");

    /* Check if file opened */
    char dpath[512];

    char newPath[512]; //We need to change the permissions of the place where we are moving the file.

    strcpy(dpath, cpath);
    strcat(dpath, "/");
    strcat(dpath, argv[2]);

    if((strcmp(argv[2], "Cliff") == 0) || (strcmp(argv[2], "Big_hole")==0)){
        strcpy(newPath, dpath);
        chmod(newPath, 0777); //We change the permissions of this special directories to forbid the use of mv.
    }

    strcat(dpath, "/");
    strcat(dpath, argv[1]);



    if((destF = open(dpath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) < 0){
        close(srcF);
        error("There was an error creating the new file to copy the information in");
    }

    while ((count = read(srcF, buffer, sizeof(buffer))) != 0){
        if(count < 0) error("There were problems reading the contents of the original file");
        write(destF, buffer, count);
    }

    if((strcmp(argv[2], "Cliff") == 0) || (strcmp(argv[2], "Big_hole")==0)){
        chmod(newPath, 0666); //We return this special directories to their normal permissions.
    }

    //Then, we delete the original file
    

    char fpath[512];
    strcpy(fpath, cpath);
    strcat(fpath, "/");
    strcat(fpath, argv[1]);

    if(unlink(fpath) < 0) error("There was a problem deleting the original file");

    exit(0);
}
