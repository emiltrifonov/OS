#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>

void printNum(uint16_t num) {
    char text[8];
    snprintf(text, 8, "%u\n", num);
    if (write(1, text, 8) != 8) {
        err(4, "cant write");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        errx(1, "args pls");
    }

    int fd = open(argv[2], O_RDONLY);
    if (fd < 0) {
        err(2, "cant open");
    }

    uint8_t print = 0;
    if (strcmp("--print", argv[1]) == 0) {
        print = 1;
    }

    uint16_t nmin = UINT16_MAX, nmax = 0;

    uint16_t num = 0;
    int read_size = 0;
    while((read_size = read(fd, &num, sizeof(num))) > 0) {
        if(print) {
            printNum(num);
            continue;
        }

        if (num < nmin) {
            nmin = num;
        }
        if (num > nmax) {
            nmax = num;
        }
    }
    if (read_size < 0) {
        err(3, "cant read");
    }

    if(strcmp("--max",argv[1]) == 0) {
        printNum(nmax);
    }
    else if (strcmp("--min", argv[1]) == 0) {
        printNum(nmin);
    }

    close(fd);

    return 0;
}
