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
        errx(2, "args pls");
    }

    int rv = execlp("ls", "ls", argv[1], (char*)NULL);
    if (rv < 0) {
        err(1, "couldnt exec");
    }

    return 0;
}
