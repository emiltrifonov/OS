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
#include <ctype.h>
#include <time.h>

const char* goal = "found it!\n";

// returns 1 on matching line, 2 on non-matching and 0 when nothing was read
uint8_t readline(int fd) {
    char c;
    char buf[4096];
    int read_size;
    int len = 0;

    while ((read_size = read(fd, &c, 1)) > 0) {
        buf[len++] = c;
        if (c == '\n') { break; }
    }
    if (read_size < 0) { err(26, "cant read"); }

    if (len == 0) { return 0; }

    buf[len] = '\0';
    if (strcmp(buf, goal) == 0) { return 1; }
    else { return 2; }
}

int main(int argc, char* argv[])
{
    if (setpgid(0,0) < 0) { err(26, "cant setpgid in parent"); }

    int pfd[argc][2];
    pid_t child_pgid = 0;

    for (int i = 1; i < argc; i++) {
        if (pipe(pfd[i]) < 0) { err(26, "cant pipe"); }

        pid_t pid = fork();
        if (pid < 0) { err(26, "cant fork"); }

        if (pid == 0) {
            close(pfd[i][0]);
            if (dup2(pfd[i][1], 1) < 0) { err(26, "cant dup"); }
            close(pfd[i][1]);

            if (i == 1) {
                if (setpgid(0,0) < 0) { err(26, "cant setpgid"); }
            }
            else {
                if (setpgid(0, child_pgid) < 0) { err(26, "cant setpgid"); }
            }

            execl(argv[i], argv[i], (char*)NULL);
            err(26, "cant exec");
        }

        if (i == 1) { child_pgid = pid; }

        close(pfd[i][1]);
    }

    for (int i = 1; i < argc; i++) {
        uint8_t res;
        while((res = readline(pfd[i][0]))) {
            if (res == 1) {
                if (killpg(child_pgid, SIGTERM) < 0) { err(26, "cant kill with SIGTERM"); }

                for (int j = 1; j < argc; j++) { wait(0); close(pfd[j][0]); }

                return 0;
            }
        }
    }

    for (int i = 1; i < argc; i++) { wait(0); close(pfd[i][0]); }
    return 1;
}
