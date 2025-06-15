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
    pid_t pid = fork();
    if (pid < 0) {
        err(1, "cant fork");
    }

    if (pid == 0) {

        for (int i = 0; i < 20; i++) {
            write(1, "child\n", 6);
         }
    }
    else {
        wait(0);
        for (int i = 0; i < 20; i++) {
            write(2, "parent\n", 7);
        }
    }

    return 0;
}
