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
    int rv = execlp("date", "date", (char*)NULL);
    if (rv < 0) {
        err(1, "couldnt exec");
    }

    return 0;
}
