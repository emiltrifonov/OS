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
    if (pipe(pfd) < 0) { err(2, "cant pipe"); }

    pid_t pid = fork();
    if (pid < 0) { err(3, "cant fork"); }

    if (pid == 0) {
        close(pfd[1]);
        dup2(pfd[0],0);
        close(pfd[0]);
        execlp("wc", "wc", "-c", (char*)NULL);
        err(5, "cant exec");
    }
    else {
        close(pfd[0]);
        int len = strlen(argv[1]);
        if (write(pfd[1], argv[1], len) != len) { err(4, "cant write"); }
        close(pfd[1]);
        wait(0);
    }

    return 0;
}
