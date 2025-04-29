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

    int lines = 0, words = 0, chars = 0;
    char c, prev;
    int read_size = 0;
    while ((read_size = read(fd, &c, 1)) > 0) {
        chars++;
        if (c == '\n') {
            lines++;
        }

        if (chars == 1 && c != ' ' && c != '\n') {
            words++;
        }

        if (chars > 1 && c != ' ' && c != '\n' && (prev == ' ' || prev == '\n')) {
            words++;
        }

        prev=c;
    }

    if (read_size < 0) {
        err(3, "couldn't read from file");
    }

    char buf[1024];
    int len = snprintf(buf , 1024, "%d %d %d %s\n", lines, words, chars, argv[1]);
    write(1, buf, len);

    close(fd);
    return 0;
}
