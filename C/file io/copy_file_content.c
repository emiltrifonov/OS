#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        errx(3, "Two file names pls");
    }
  
    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        err(1, "couldn't open file for reading");
    }

    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd2 < 0) {
        err(2, "couldn't open file for writing");
    }

    const size_t buf_size = 4096;
    char buf[buf_size];
    int read_size = 0;
    while ((read_size = read(fd1, buf, buf_size)) > 0) {
        write(fd2, buf, read_size);
    }

    if (read_size < 0) {
        err(4, "couldn't read from file");
    }

    close(fd1);
    close(fd2);
    return 0;
}
