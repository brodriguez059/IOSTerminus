#include "defines.h"

int main(int argc, char* argv[])
{
   //We will concatenate the contents of two files into a new one.

   //We check that everything is ok.
   if (argc < 3) {
      write(STDOUT, "Hmm... you should try to use more arguments, yes?\n", 51);
      exit(1);
   }

   //We open the two files and create the new one.
   int fd1;
   if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
      error("There was an error opening the file");
   }
   int fd2;
   if ((fd2 = open(argv[2], O_RDONLY)) == -1) {
      error("There was an error opening the file");
   }
   char filename[512];
   strcpy(filename, argv[1]);
   strcat(filename, "_");
   strcat(filename, argv[2]);

   int fd_new = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
   if(fd_new == -1){
      error("There was a problem creating the new item\n");
   }

   //We read the first file and put its content into the new one.
   char c;
   while(read(fd1,&c,1) > 0){
      if(write(fd_new,&c,1) < 0){
         error("There was a problem reading from the file");
      }
   }
   //We read the second file and put its content into the new one.
   while(read(fd2,&c,1) > 0){
      if(write(fd_new,&c,1) < 0){
         error("There was a problem reading from the file");
      }
   }

   //We delete the two files.
   close(fd1);
   close(fd2);

   char* cwd = getcwd(NULL, 0);
   char f1_path[512];
   char f2_path[512];

   strcpy(f1_path, cwd);
   strcat(f1_path,"/");
   strcat(f1_path, argv[1]);
   strcpy(f2_path, cwd);
   strcat(f2_path,"/");
   strcat(f2_path, argv[2]);

   char rollbackPath1[512]; //We prepare a special path in order to rollback the concatenated file and
   char rollbackPath2[512]; //we prepare another special path in order to rollback the concatenated file
   strcpy(rollbackPath1, cwd); //in case it fails to delete one of the originals.
   strcat(rollbackPath1,"/");

   strcpy(rollbackPath2, rollbackPath1);

   strcat(rollbackPath1, filename);

   //printf("Our cwd: %s\n",cwd);
   //printf("The file to rollback: %s\n", rollbackPath1);

   if(unlink(f1_path) < 0){
      strcat(rollbackPath2,argv[2]);
      printf("The new name is: %s\n", rollbackPath2);
      rename(rollbackPath1, rollbackPath2);
      error("There were problems to fuse the first file");
   };
   if(unlink(f2_path) < 0){
      strcat(rollbackPath2,argv[1]);
      printf("The new name is: %s\n", rollbackPath2);
      rename(rollbackPath1, rollbackPath2);
      error("There were problems to fuse the second file");
   }
   
   return 0;
}
