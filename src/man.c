#include <fcntl.h>

#include "defines.h"

dir_t gameDirs[NUMDIR];

int main (int argc, char *argv[]) {

	int fd;
	char c;

    char path[512];

    strcpy(path, gameDirs[DOCS].name);
    strcat(path, argv[1]);
    strcat(path,".txt");

    fd= open(path, O_RDONLY);

    if(fd < 0) {
		error("There was an error opening the file")
	}

	while(read(fd, &c, 1) > 0){
		if(write(STDOUT, &c, 1) < 0){
			error("There was an error reading the file")
		}
	}

	exit(0);
}