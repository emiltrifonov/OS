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
    if (argc != 3) { errx(1, "bad args"); }

    pid_t pid = fork();
    if (pid < 0) { err(2, "cant fork"); }

    if (pid == 0) {
        execlp(argv[1], argv[1], (char*)NULL);
    }
    else {
        int status;
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { exit(42); }

        execlp(argv[2], argv[2], (char*)NULL);
    }

    return 0;
}
