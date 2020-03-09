// stee.c

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
   int fd;
   char c;

   if (argc != 2) {
      write(2, "Usage: stee output_file\n", 24);
      exit(1);
   }

   if ((fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1) {
      write(2, "ERROR open\n", 11);
      exit(1);
   }
   while (read(0, &c, 1) > 0) {
      write(1, &c, 1);
      write(fd, &c, 1);
   }
   close(fd);
}
