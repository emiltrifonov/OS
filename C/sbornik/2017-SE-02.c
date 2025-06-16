#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>

uint8_t enum_lines = 0;
int start_ind = 1;
uint32_t n = 1;

void printLineNum() {
    char txt[11];
    snprintf(txt, 11, "%u. ", n);
    int len = strlen(txt);
    if (write(1, txt, len) != len) { err(3, "cant write"); }
}

void readFromFD(int fd) {
    uint8_t b, prev = '\n';
    int read_size;
    while((read_size = read(fd, &b, 1)) > 0) {
        if (enum_lines == 1 && prev == '\n') {
            printLineNum();
            n++;
        }
        if (write(1, &b, sizeof(b)) != sizeof(b)) { err(2, "cant write"); }
        prev = b;
    }
    if (read_size < 0) { err(4, "cant read"); }
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        readFromFD(0);
        exit(0);
    }
    if (strcmp(argv[1], "-n") == 0) {
        enum_lines = 1;
        start_ind = 2;
    }
    if (start_ind == argc) {
        readFromFD(0);
        exit(0);
    }

    for (int i = start_ind; i < argc; i++) {
        if (strcmp(argv[i], "-") == 0) {
            readFromFD(0);
        }
        else {
            int fd = open(argv[i], O_RDONLY);
            if (fd < 0) { err(1, "cant open"); }
            readFromFD(fd);
            close(fd);
        }
    }

    return 0;
}
