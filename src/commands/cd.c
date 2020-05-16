#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

#define BUF_SIZE 1024
#define PATH_LIMIT 512
#define NO_SUCH_DIRECTORY "There is no room called "
#define NO_ACCESS_PERMISIONS "There is a strange force preventing you from going there"
#define OUTPUT 1
#define ERR_OUTPUT 2

void cd(char *path);

int main(int argc, char *argv[]) //test run 
{
	char buf[BUF_SIZE] = { 0 };
	char currentPath[PATH_LIMIT];
	getcwd(currentPath, PATH_LIMIT);
	write(OUTPUT, "Current directory:", 18);
	write(OUTPUT, currentPath, strlen(currentPath));
	write(OUTPUT, "\n", 1);

   	cd(argv[1]);
	
  	getcwd(currentPath, PATH_LIMIT);
	write(OUTPUT, "Mounted directory:", 18);
	write(OUTPUT, currentPath, strlen(currentPath));
	write(OUTPUT, "\n", 1);

	return 0;
}


void cd(char *path) { // function to be used by parent process
	char buf[BUF_SIZE] = { 0 };
	char currentPath[PATH_LIMIT];
	getcwd(currentPath, PATH_LIMIT); //system call which sets current path with current path
	strcpy(buf, currentPath); // put the path to buffer
	if (strcmp(path, "..") == 0)
	{
		char currentPathTokenized[PATH_LIMIT];
		strcpy(currentPathTokenized, currentPath);
		char *token;
		char *lastToken;
		token = strtok(currentPathTokenized, "/");
		while (token != NULL)
		{
			lastToken = token;
			token = strtok(NULL, "/");
		}
		if (strcmp(lastToken, "Home") == 0)
		{
			strcpy(buf, NO_ACCESS_PERMISIONS);
			write(OUTPUT, buf, strlen(buf));
			write(OUTPUT, "\n", 1);
			return;
		}
	}
	strcat(buf, "/");
	strcat(buf, path); // add the path we want to go to
	if (chdir(buf) == -1) { // system call which changes process directory
		if (errno == ENOENT || errno == ENOTDIR) { // if no such file or is not a directory
			strcpy(buf, NO_SUCH_DIRECTORY);
			strcat(buf, path);
			write(OUTPUT, buf, strlen(buf));
			write(OUTPUT, "\n", 1);
		}
		else if (errno == EACCES || errno == EPERM) { // if no permissions for the directory
			strcpy(buf, NO_ACCESS_PERMISIONS);
			write(OUTPUT, buf, strlen(buf));
			write(OUTPUT, "\n", 1);
		} else {
			strcpy(buf, strerror(errno));
			write(ERR_OUTPUT, buf, strlen(buf));
			write(OUTPUT, "\n", 1);
		}
	}
	munmap(currentPath, PATH_LIMIT); // system call to release memory allocated by getcwd
	memset(buf, '\0', BUF_SIZE); // clean the buffer for file reading and writing, unsure if allowed
	int fd = open(".description", O_RDONLY);
	if (fd == -1) {
		write(OUTPUT, "Error opening File .description in the folder: ", 38);
		strcpy(buf, strerror(errno));
		write(ERR_OUTPUT, buf, strlen(buf));
		write(OUTPUT, "\n", 1);
		return;
	}
	for(;;) {
		int res = read(fd, buf, BUF_SIZE);
		if( res > 0) {
			write(OUTPUT, buf, BUF_SIZE);
		} else if (res == -1) {
			strcpy(buf, strerror(errno));
			write(ERR_OUTPUT, buf, strlen(buf));
			write(OUTPUT, "\n", 1);
		} else {
			break;
		}
	}
	close(fd);
}
