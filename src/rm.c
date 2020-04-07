#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define error(a) {perror(a); exit(1)};

int main (int argc, char *argv[]) {
   int fd;

   fd = open(argv[1], O_RDONLY);
   close(fd);
   
   remove(argv[1]);

   if(fd < 0) {
      error("Error: unable to delete the file");
   }
   
   return(0);
}



