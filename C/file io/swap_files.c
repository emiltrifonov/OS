#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        errx(1, "Two file names pls");
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        err(2, "cant read");
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 < 0) {
        err(3, "cant read");
    }

    int tempfd = open("my_temp_file", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (tempfd < 0) {
        err(4, "cant create temp file");
    }

    const size_t buf_size = 4096;
    char buf[buf_size];
    int read_size = 0;
    // read entire fd1 and write to tempfd
    while ((read_size = read(fd1, &buf, buf_size)) > 0) {
        if (write(tempfd, &buf, read_size) != read_size) {
            err(6, "couldnt write");
        }
    }
    if (read_size < 0) {
        err(5, "couldnt read");
    }

    close(fd1);
    fd1 = open(argv[1], O_WRONLY | O_TRUNC);
    if (fd1 < 0) {
        err(14, "cant open");
    }
  
    // read entire fd2 and write to fd1
    while ((read_size = read(fd2, &buf, buf_size)) > 0) {
        if (write(fd1, &buf, read_size) != read_size) {
            err(8, "couldnt write");
        }
    }
    if (read_size < 0) {
        err(9, "couldnt read");
    }

    close(fd2);
    fd2 = open(argv[2], O_WRONLY | O_TRUNC);
    if (fd2 < 0) {
        err(15, "couldnt open");
    }

    int pos = lseek(tempfd, 0, SEEK_SET);
    if (pos < 0) {
        err(11, "couldnt reposition");
    }

    // read entire tempfd and write to fd2
    while ((read_size = read(tempfd, &buf, buf_size)) > 0) {
        if (write(fd2, &buf, read_size) != read_size) {
            err(13, "couldnt write");
        }
    }
    if (read_size < 0) {
        err(12, "couldnt read");
    }

    close(fd1);
    close(fd2);
    close(tempfd);

    return 0;
}
