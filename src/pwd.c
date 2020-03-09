#include <unistd.h>
#include <errno.h>
#include <string.h>
#define BUFFER_SIZE 128
#define STDOUT 1

int getCurrentWorkingDirectory(char *buf) {
  if (getcwd(buf, BUFFER_SIZE) == NULL) {
        char *error;
        switch(errno) {
            case EACCES:
                error = "Permission to read denied.\n";
                break;
            case EFAULT:
                error = "Bad address.\n";
                break;
            case EINVAL:
                error = "Buffer is not a null pointer or size is zero.\n";
                break;
            case ENAMETOOLONG:
                error = "Pathname string exceeds PATH_MAX bytes.\n";
                break;
            case ENOENT:
                error = "Directory was unlinked.\n";
                break;
            case ENOMEM:
                error = "Out of memory.\n";
                break;
            case ERANGE:
                error = "Size argument is less than the length of the pathname.\n";
                break;
            default:
                error = "Unknown error.\n";
        }
    write(STDOUT, error, strlen(error));
    return 1;

  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {
  char cwd[BUFFER_SIZE];
  int rCode = getCurrentWorkingDirectory(cwd);
  if (rCode == 0) {
    char out[strlen(cwd) + 2];
    strcpy(out, cwd);
    strcat(out, "\n");
    write(STDOUT, out, strlen(out));
  } else {
    // TOOD: manage error
  }
}

