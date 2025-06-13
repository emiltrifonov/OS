#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>

int main(void)
{
    int fd1 = open("/etc/passwd", O_RDONLY);
    if (fd1 < 0) {
        err(1, "cant open");
    }

    int fd2 = open("passwd", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd2 < 0) {
        err(2, "cant open");
    }

    char c;
    int read_size = 0;
    while ((read_size = read(fd1, &c, 1)) > 0) {
        if (c == ':') {
            c = '?';
        }

        if (write(fd2, &c, read_size) != read_size) {
            err(3, "cant write");
        }
    }
    if (read_size < 0) {
        err(4, "cant read");
    }

    close(fd1);
    close(fd2);

    return 0;
}
