
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "defines.h"

#define error(a) {perror(a); exit(1);}

int main(int argc, char* argv[]) {

int fd;
char c;

	fd = open("help.txt",O_RDONLY);
	
	if(fd < 0) {
		error("There was an error opening the file")
	}

	while(read(fd, &c,sizeof(c)!=0)){
		if(write(STDIN, &c, sizeof(c)) < 0){
			error("There was an error reading the file")
		}
		//printf("%c",c); Not a syscall.
	}


	close(fd);
	exit(0);
 
}