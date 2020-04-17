#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
 
#define BUFF_SIZE 1024
#define OUTPUT 1 
#define ERR_OUTPUT 2

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		write(ERR_OUTPUT, "cp needs 2 arguments \"cp <<source path>> <<destination path>>\"\n", 63);
		return 1;
	}

    char buff[BUFF_SIZE] = { 0 };
	int source = open(argv[1],O_RDONLY);
	if(source == -1)
	{
		write(ERR_OUTPUT, "Source: ", 8);
    		strcpy(buff, strerror(errno));
		write(ERR_OUTPUT, buff, strlen(buff));
		write(ERR_OUTPUT, "\n", 1);
		return 1;
	}

    // destination file is writeonly, is created if not there, truncated otherwise and has all read write user group all bytes
    int destination = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(destination == -1)
	{
		write(ERR_OUTPUT, "Destination: ", 13);
	    	strcpy(buff, strerror(errno));
		write(ERR_OUTPUT, buff, strlen(buff));
		write(ERR_OUTPUT, "\n", 1);
		return 1;
	}

	int bytesRead;
	int bytesWritten;
	while((bytesRead = read(source,buff,BUFF_SIZE)) > 0)
	{
		if(write(destination,buff,bytesRead) != bytesRead)
		{
			write(ERR_OUTPUT, "Writting: ", 10);
			strcpy(buff, strerror(errno));
			write(ERR_OUTPUT, buff, strlen(buff));
			write(ERR_OUTPUT, "\n", 1);
    		return 1;
		}
	}
	
	if(bytesRead == -1)
	{
		write(ERR_OUTPUT, "Reading: ", 9);
		strcpy(buff, strerror(errno));
		write(ERR_OUTPUT, buff, strlen(buff));
		write(ERR_OUTPUT, "\n", 1);
		return 1;
	}
	
	if(close(source) == -1)
	{
		write(ERR_OUTPUT, "Source: ", 8);
		strcpy(buff, strerror(errno));
		write(ERR_OUTPUT, buff, strlen(buff));
		write(ERR_OUTPUT, "\n", 1);
		return 1;
	}
 
	if(close(destination) == -1)
	{
		write(ERR_OUTPUT, "Destination: ", 13);
		strcpy(buff, strerror(errno));
		write(ERR_OUTPUT, buff, strlen(buff));
		write(ERR_OUTPUT, "\n", 1);
		return 1;
	}
    return 0;
}
