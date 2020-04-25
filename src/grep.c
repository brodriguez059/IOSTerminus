#include <string.h>
#include "defines.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        const char *error = "Missing parameters.\nUsage: grep <haystack> <needle>\n";
        write(STDERR, error, strlen(error));
        return 1;
    }

    const char *haystack = argv[1];
    const char *needle = argv[2];
    char *txt;

    txt = strstr(haystack, needle);

    if (txt) {
        write(STDOUT, txt, strlen(txt));
        write(STDOUT, "\n", strlen("\n"));
    }

    return 0;
}
