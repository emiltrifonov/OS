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
    if (argc != 3) {
        errx(1, "two args pls");
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        return 0;
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        err(2, "cant open");
    }

    int fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd2 < 0) {
        err(3, "cant open");
    }

    const size_t buf_size = 4096;
    char buf[buf_size];
    int read_size = 0;
    while ((read_size = read(fd1, buf, buf_size)) > 0) {
        if (write(fd2, buf, read_size) != read_size) {
            err(4, "cant write");
        }
    }
    if (read_size < 0) {
        err(5, "cant read");
    }

    close(fd1);
    close(fd2);

    return 0;
}
