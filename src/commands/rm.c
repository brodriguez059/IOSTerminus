#include "defines.h"

int main (int argc, char *argv[]) {
   if (argc < 2) {
       write(STDOUT, "Hmm... you should try to use more arguments, yes?\n", 51);
       exit(1);
    }

   char* parentPath = getcwd(NULL, 0); 
   char path[512];

   strcpy(path, parentPath);
   strcat(path,"/");
   strcat(path, argv[1]);

   //if(fd < 0) {
   if(unlink(path) < 0) {
      error("Error: unable to delete the file");
   }
   
   return(0);
}



