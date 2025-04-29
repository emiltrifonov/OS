#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        err(1, "only 1 file name pls");
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        err(2, "couldn't open file for reading");
    }

    char c;
    int read_size = 0;
    int rows = 0;
    while (rows < 10 && (read_size = read(fd, &c, 1)) > 0) {
        write(1, &c, 1);
        if (c == '\n') {
            rows++;
        }
    }

    if (read_size < 0) {
        err(3, "couldn't read from file");
    }

    close(fd);
    return 0;
}
