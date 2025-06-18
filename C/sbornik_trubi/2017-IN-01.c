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

int main(void)
{
    int pfd1[2], pfd2[2], pfd3[2];
    if (pipe(pfd1) < 0) { err(1, "cant pipe"); }
    if (pipe(pfd2) < 0) { err(1, "cant pipe"); }
    if (pipe(pfd3) < 0) { err(1, "cant pipe"); }

    pid_t pid = fork();
    if (pid < 0) { err(2, "cant fork"); }

    if (pid == 0) {
        close(pfd1[0]);
        close(pfd2[0]);
        close(pfd2[1]);
        close(pfd3[0]);
        close(pfd3[1]);

        if (dup2(pfd1[1], 1) < 0) { err(3, "cant dup"); }
        close(pfd1[1]);

        execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL);
        err(4, "cant exec");
    }

    pid = fork();
    if (pid < 0) { err(3, "cant fork"); }

    if (pid == 0) {
        close(pfd2[0]);
        close(pfd1[1]);
        close(pfd3[0]);
        close(pfd3[1]);

        if (dup2(pfd1[0], 0) < 0) { err(5, "cant dup"); }
        close(pfd1[0]);
        if (dup2(pfd2[1], 1) < 0) { err(6, "cant dup"); }
        close(pfd2[1]);

        execlp("sort", "sort", (char*)NULL);
        err(7, "cant exec");
    }

    pid = fork();
    if (pid < 0) { err(8, "cant fork"); }

    if (pid == 0) {
        close(pfd1[0]);
        close(pfd1[1]);
        close(pfd3[0]);
        close(pfd2[1]);

        if (dup2(pfd2[0], 0) < 0) { err(9, "cant dup"); }
        close(pfd2[0]);
        if (dup2(pfd3[1], 1) < 0) { err(10, "cant dup"); }
        close(pfd3[1]);

        execlp("uniq", "uniq", "-c", (char*)NULL);
        err(11, "cant exec");
    }

    pid = fork();
    if (pid < 0) { err(12, "cant fork"); }

    if (pid == 0) {
        close(pfd1[0]);
        close(pfd1[1]);
        close(pfd2[0]);
        close(pfd2[1]);
        close(pfd3[1]);

        if (dup2(pfd3[0], 0) < 0) { err(12, "cant dup"); }
        close(pfd3[0]);

        execlp("sort", "sort", "-n", (char*)NULL);
        err(14, "cant exec");
   }

    close(pfd1[0]); close(pfd1[1]);
    close(pfd2[0]); close(pfd2[1]);
    close(pfd3[0]); close(pfd3[1]);
    
    wait(0);
    wait(0);
    wait(0);
    wait(0);
    return 0;
}
