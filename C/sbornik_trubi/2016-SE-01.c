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

    int pfd[2];
    if (pipe(pfd) < 0) { err(3, "cant pipe"); }

    pid_t pid = fork();
    if (pid < 0) { err(4, "cant fork"); }

    if (pid == 0) {
        close(pfd[0]);
        if (dup2(pfd[1], 1) < 0) { err(5, "cant dup"); }
        close(pfd[1]);
        execlp("cat", "cat", argv[1], (char*)NULL);
        err(6, "cant exec");
    }
    else {
        wait(0);
        close(pfd[1]);
        if (dup2(pfd[0], 0) < 0) { err(7, "cant dup"); }
        close(pfd[0]);
        execlp("sort", "sort", (char*)NULL);
        err(8, "cant exec");
    }

    return 0;
}
