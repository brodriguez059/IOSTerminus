#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define STDOUT 1
#define STDERR 2

/*
// event listener
void fileCreated(char *filename) {
    write(STDOUT, "File ", strlen("File "));
    write(STDOUT, filename, strlen(filename));
    write(STDOUT, " created\n", strlen(" created\n"));
}

int main(int argc, char* argv[]) {
    mainReal(argc, argv, fileCreated);
    // mainReal(argc, argv, NULL);
}
int mainReal(int argc, char* argv[], void fileCreated(char*)) {
*/

int main(int argc, char* argv[]) {
    if (argc == 1) {
        char *buf = "Usage: touch <file name>\n";
        write(STDERR, buf, strlen(buf));

        return 1;
    }

    char *filename = argv[1];

    if (creat(filename, 0666) == -1)  {

        char *error;
        switch (errno) {
            case EACCES:
                error = "Permission denied.";
                break;
            case EDQUOT:
                error = "User's quota of disk blocks or inodes on the file system has been exhausted.";
                break;
            case EEXIST:
                error = "File already exist.";
                break;
            case EFAULT:
                error = "The given path points outside the accessible address space.";
                break;
            case EFBIG:
            case EOVERFLOW:
                error = "File is too large to be opened.";
                break;
            case EINTR:
                error = "The call was interrupted by a signal handler while creating the file.";
                break;
            case EISDIR:
                error = "The given path refers to a directory.";
                break;
            case ELOOP:
                error = "Too many symbolic links were encountered (possible infinite loop?).";
                break;
            case EMFILE:
                error = "Process has reached maximun number of opened files.";
                break;
            case ENAMETOOLONG:
                error = "Path name is too long.";
                break;
            case ENFILE:
                error = "The system limit on the total number of open files has been reached.";
                break;
            case ENODEV:
                error = "WOW! You triggered a Linux kernel bug, WP.";
                break;
            case ENOENT:
                error = "A directory component of the given path does not exist.";
                break;
            case ENOMEM:
                error = "Insufficient kernel memory was available.";
                break;
            case ENOSPC:
                error = "The device containing the new file has no room.";
                break;
            case ENOTDIR:
                error = "A component used as a directory in pathname is not.";
                break;
            case ENXIO:
                error = "The named file is a FIFO and no process has the file open for reading. Or, the file is a device special file and no corresponding device exists.";
                break;
            case EPERM:
                error = "The O_NOATIME flag was specified, but the effective user ID of the caller did not match the owner of the file and the caller was not privileged (CAP_FOWNER).";
                break;
            case EROFS:
                error = "Cannot write on a read-only file system.";
                break;
            case ETXTBSY:
                error = "Given pathname refers to an executable file which is currently being executed.";
                break;
            case EWOULDBLOCK:
                error = "The O_NONBLOCK flag was specified, and an incompatible lease was held on the file.";
                break;
        }
        char *prefix = "[ERROR] ";
        write(STDERR, prefix, strlen(prefix));
        write(STDERR, error, strlen(error));
        write(STDERR, "\n", strlen("\n"));

        return 2;
    }

    /*
    // execute event
    if (fileCreated != NULL) {
        fileCreated(filename);

        return 0;
    }
    */

    char *buf = "File created\n";
    write(STDOUT, buf, strlen(buf));

    return 0;
}
