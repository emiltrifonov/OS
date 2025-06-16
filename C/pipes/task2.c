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

    int pid = fork();
    if (pid < 0) { err(3, "cant fork"); }

    if (pid == 0){
        close(pfd[1]);

        const size_t bs = 4096;
        char buf[bs];
        int read_size = 0;
        while ((read_size = read(pfd[0], buf, bs)) > 0) {
            if (write(1, buf, read_size) != read_size) { err(4, "cant write"); }
        }
        if (read_size < 0) { err(5, "cant read"); }

        close(pfd[0]);
        exit(0);
    }
    else {
        close(pfd[0]);

        int len = strlen(argv[1]);
        if (write(pfd[1], argv[1], len) != len) { err(6, "cant write"); }

        close(pfd[1]);
    }

    return 0;
}
