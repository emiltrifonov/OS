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

uint8_t inrange(int num, int start, int len) {
    return num >= start && num <= (start + len - 1);
}

void c(int start, int len) {
    char c;
    int read_size;
    int counter = 1;
    while ((read_size = read(0, &c, 1)) > 0) {
        if(c == '\n' || inrange(counter, start, len)) {
            if (write(1, &c, 1) != 1) { err(3, "cant write"); }
            if (c == '\n') {
                counter = 1;
                continue;
            }
        }

        counter++;
    }
    if (read_size < 0) { err(2, "cant read"); }
}

void df(char sep, int start, int len) {
    int fieldnum = 1;
    char c;
    int read_size;
    while((read_size = read(0, &c, 1)) > 0) {
        if (c == sep) {
            if (inrange(fieldnum, start, len) && inrange(fieldnum+1, start, len) && write(1, &c, 1) != 1) { err(7, "cant write"); }
            fieldnum++;
        }
        else if (c == '\n') {
            if (write(1, &c, 1) != 1) { err(5, "cant write"); }
            fieldnum = 1;
        }
        else if (inrange(fieldnum, start, len)) {
            if (write(1, &c, 1) != 1) { err(6, "cant write"); }
        }
    }
    if (read_size < 0) { err(4, "cant read"); }
}

uint8_t isdigit(char c) {
    return c >= '1' && c <= '9';
}

uint8_t isrange(char* arg) {
    return isdigit(arg[0]) && isdigit(arg[2]) && arg[1] == '-';
}

int main(int argc, char* argv[])
{
    if (argc == 3 && strcmp(argv[1], "-c") == 0) {
        int len = strlen(argv[2]);
        if ( (len == 1 && isdigit(argv[2][0])) || (len == 3 && isrange(argv[2])) ) {
            int start = argv[2][0]-'0';
            if (len == 1) {
                c(start, 1);
            }
            else {
                c(start, argv[2][2] - argv[2][0] + 1);
            }
            return 0;
        }
    }
    else if (argc == 5 && strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-f") == 0) {
        char sep = argv[2][0];
        int len = strlen(argv[4]);
        if ( (len == 1 && isdigit(argv[4][0])) || (len == 3 && isrange(argv[4])) ) {
            int start = argv[4][0]-'0';
            if (len == 1) {
                df(sep, start, 1);
            }
            else {
                df(sep, start, argv[4][2] - argv[4][0] + 1);
            }
            return 0;
        }
    }

    errx(1, "bad args");
}
