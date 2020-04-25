#include <unistd.h>
#include <errno.h>
#include <string.h>
// import PATH_MAX (max path length)
#include <linux/limits.h>
#define STDOUT 1
#define STDERR 2

int getCurrentWorkingDirectory(char *buf, size_t size) {
    if (getcwd(buf, size) == NULL) {
        char *error;
        switch(errno) {
            case EACCES:
            error = "Permission to read denied.";
            break;
            case EFAULT:
            error = "Bad address.";
            break;
            case EINVAL:
            error = "Buffer is not a null pointer or size is zero.";
            break;
            case ENAMETOOLONG:
            error = "Pathname string exceeds PATH_MAX bytes.";
            break;
            case ENOENT:
            error = "Directory was unlinked.";
            break;
            case ENOMEM:
            error = "Out of memory.";
            break;
            case ERANGE:
            error = "Size argument is less than the length of the pathname.";
            break;
            default:
            error = "Unknown error.";
        }
        strcpy(buf, error);
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[]) {
    char cwd[PATH_MAX];
    int rCode = getCurrentWorkingDirectory(cwd, sizeof(cwd));

    if (rCode == 0) {
        const char *sep = "/";

        char *dirname = strtok(cwd, sep);
        char lastDirname[strlen(cwd)];

        while(dirname) {
            strcpy(lastDirname, dirname);
            dirname = strtok(NULL, sep);
        }

        const char *phraseStart = "You're currently in \"";
        const char *phraseEnd = "\".\n";

        write(STDOUT, phraseStart, strlen(phraseStart));
        write(STDOUT, lastDirname, strlen(lastDirname));
        write(STDOUT, phraseEnd, strlen(phraseEnd));

    } else {
        const char *prefix = "[ERROR] ";
        write(STDERR, prefix, strlen(prefix));

        char out[strlen(cwd)];
        strcpy(out, cwd);
        strcat(out, "\n");

        write(STDERR, out, strlen(out));
    }

    return rCode;
}
