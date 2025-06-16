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

int main(int argc, char* argv[])
{
    if (argc != 2) { errx(1, "bad args"); }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) { err(2, "cant open"); }

    struct stat st;
    if (fstat(fd, &st) < 0) { err(3, "fstat brr"); }

    size_t size = st.st_size;

    for (size_t i = 0; i < size - 1; i++) {
        if (lseek(fd, i, SEEK_SET) < 0) { err(10, "cant repos"); }

        //save current num
        uint8_t currnum;
        if (read(fd, &currnum, 1) < 0) { err(5, "cant read"); }

        uint8_t currmin = currnum, ch;
        size_t minind = i;
        for (size_t j = i + 1; j < size; j++) {
            if (read(fd, &ch, 1) < 0) { err(4, "cant read"); }

            if (ch < currmin) {
                currmin = ch;
                minind = j;
            }
        }

        if (minind != i) {
            if (lseek(fd, i, SEEK_SET) < 0) { err(7, "cant repos"); }

            if (write(fd, &currmin, 1) < 0) { err(8, "cant write"); }

            if (lseek(fd, minind, SEEK_SET) < 0) { err(9, "cant repos"); }

            if (write(fd, &currnum, 1) < 0) { err(9, "cant write"); }
        }
    }

    close(fd);

    return 0;
}
