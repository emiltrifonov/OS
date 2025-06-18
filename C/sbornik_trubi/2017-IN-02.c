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

void reset(char vp[3][5]) {
    for (int i = 1; i < 3; i++) {
        strcpy(vp[i], "\0\0\0\0\0");
    }
}

uint8_t isSep(char c) {
    return c == 0x20 || c == 0x0A;
}

void changePar(int* par) {
    if ((*par) == 1) {
        (*par) = 2;
    }
    else {
        (*par) = 1;
    }
}

void execute(char vp[3][5], int argc) {
    pid_t pid = fork();
    if (pid < 0) { err(10, "cant fork"); }

    if (pid == 0) {
        if (argc == 1) {
            execlp(vp[0], vp[0], vp[1], (char*)NULL);
        }
        else {
            execlp(vp[0], vp[0], vp[1], vp[2], (char*)NULL);
        }
        err(11, "cant exec");
    }

    wait(0);
    reset(vp);
}

int main(int argc, char* argv[])
{
    char vp[3][5];
    strcpy(vp[0], "echo\0");
    if (argc > 2 || (argc == 2 && strlen(argv[1]) > 4)) { errx(1, "bad args"); }
    if (argc == 2 && strlen(argv[1]) <= 4) { strcpy(vp[0], argv[1]); }

    reset(vp);
    int par_len = 0; int curpar = 1;
    char c;
    int read_size;
    while ((read_size = read(0, &c, 1)) > 0) {
        if (isSep(c)) {
            par_len = 0;

            if (curpar == 2) {
                execute(vp, curpar);
            }

            changePar(&curpar);
            continue;
        }

        if (par_len == 4) {
            errx(2, "args cant be longer than 4");
        }

        vp[curpar][par_len++] = c;
    }
    if (read_size < 0) { err(5, "cant read"); }

    // curpar = 2 means we already have 1 argument loaded
    if (curpar == 2) {
        execute(vp, 1);
    }

    return 0;
}
