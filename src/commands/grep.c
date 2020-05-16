#include <errno.h>
#include <string.h>
#include "defines.h"

#include <stdlib.h>

void handleOpenError() {
    char *error = "Error opening file: ";
    write(STDOUT, error, strlen(error));
    write(STDOUT, strerror(errno), strlen(strerror(errno)));
    write(STDOUT, "\n", strlen("\n"));
}

void handleReadError() {
    char *error = "Error reading file: ";
    write(STDOUT, error, strlen(error));
    write(STDOUT, strerror(errno), strlen(strerror(errno)));
    write(STDOUT, "\n", strlen("\n"));
}

void handleCloseError() {
    char *error = "Error closing file: ";
    write(STDOUT, error, strlen(error));
    write(STDOUT, strerror(errno), strlen(strerror(errno)));
    write(STDOUT, "\n", strlen("\n"));
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        const char *error = "Missing parameters.\nUsage: grep <file> <needle>\n";
        write(STDERR, error, strlen(error));
        return 1;
    }

    const char *filename = argv[1];
    const char *needle = argv[2];

    const int fd = open(filename, O_RDONLY);

    if (fd > -1) {
        int readBytes;
        char *c = malloc(1);
        unsigned int commonChars = 0;

        while((readBytes = read(fd, c, 1)) > 0) {

            char n = needle[commonChars];

            if (commonChars < strlen(needle) && (unsigned int) abs(strcmp(c, &n)) == commonChars) {
                commonChars = commonChars + 1;

                if (commonChars == strlen(needle)) {
                    write(STDOUT, needle, strlen(needle));
                }

            } else if (commonChars == strlen(needle)) {
                write(STDOUT, c, strlen(c));

            } else {
                commonChars = 0;
            }

            if (strcmp(c, "\n") == 0) {
                commonChars = 0;
            }
        }

        write(STDOUT, "\n", strlen("\n"));

        if (readBytes == -1) {
            handleReadError();
            return 2;
        }

        if (close(fd) == -1) {
            handleCloseError();
            return 3;
        }

    } else {
        handleOpenError();
        return 1;
    }

    return 0;
}
