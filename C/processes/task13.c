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
    if (argc != 3) { errx(10, "bad args"); }

    int done = 0;
    pid_t pid1 = fork();
    if (pid1 < 0) { err(1, "cant fork"); }

    if (pid1 == 0) {
        execlp(argv[1], argv[1], (char*)NULL);
        exit(127);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) { err(2, "cant fork"); }

    if (pid2 == 0) {
        execlp(argv[2], argv[2], (char*)NULL);
        exit(127);
    }

    int pid;
    int status;
    for (int i = 0; i < 2; i++) {
        pid = wait(&status);
        if (done == 0 && WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            char txt[16];
            snprintf(txt, 16, "%d\n", pid);
            int len = strlen(txt);
            if (write(1, txt, len) != len) { err(3, "cant write"); }
            done = 1;
        }
    }

    if (done == 0) {
        if (write(1, "-1\n", 3) != 3) { err(4, "cant write"); }
    }

    return 0;
}
