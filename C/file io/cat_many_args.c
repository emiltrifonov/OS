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
    const size_t buf_size = 4096;
    char buf[buf_size];
    int read_size = 0;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            err(1, "cant open");
        }
        while ((read_size = read(fd, buf, buf_size)) > 0) {
            if (write(1, buf, read_size) != read_size) {
                err(2, "cant write");
            }
        }
        if (read_size < 0) {
            err(3, "cant read");
        }
        close(fd);
    }

    return 0;
}
