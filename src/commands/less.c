#include "defines.h"

int main(int argc, char* argv[])
{
   int fd;
   char c;

   if (argc != 2) {
      write(2, "Usage: less [file name]\n", 24);
      exit(1);
   }

   if ((fd = open(argv[1], O_RDONLY)) == -1) {
      error("There was an error opening the file");
   }
   while (read(fd, &c, 1) > 0) {
      write(1, &c, 1);
   }
   close(fd);

   exit(0);
}
