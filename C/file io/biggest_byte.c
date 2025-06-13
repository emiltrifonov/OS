#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        errx(1, "args pls");
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        err(2, "cant open");
    }

    uint8_t bmax = 0;

    uint8_t byte = 0;
    int read_size = 0;
    while((read_size = read(fd, &byte, sizeof(byte))) > 0) {
        if (byte > bmax) {
            bmax = byte;
        }
    }
    if (read_size < 0) {
        err(3, "cant read");
    }

    char text[5];
    snprintf(text, 5, "%u\n", byte);
    if (write(1, text, 5) != 5) {
        err(4, "cant write");
    }

    close(fd);

    return 0;
}
