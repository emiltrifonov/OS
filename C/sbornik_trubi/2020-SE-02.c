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

int main(int argc, char* argv[])
{
    if (argc != 3) { errx(1, "bad args"); }

    int pfd[2];
    if (pipe(pfd) < 0) { err(2, "cant pipe"); }

    pid_t pid = fork();
    if (pid < 0) { err(3, "cant fork"); }

    if (pid == 0) {
        close(pfd[0]);
        if (dup2(pfd[1], 1)) { err(4, "cant dup"); }
        close(pfd[1]);
        execlp("cat", "cat", argv[1], (char*)NULL);
        err(5, "cant exec");
    }

    close(pfd[1]);
    if (dup2(pfd[0], 0) < 0) { err(6, "cant dup"); }

    uint8_t c;
    int read_size;

    int out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { err(7, "cant open"); }

    while((read_size = read(0, &c, 1)) > 0) {
        if (c == 0x55) { continue; }

        if (c == 0x7D) {
            if (read(0, &c, 1) != 1) { err(8, "cant read"); }

            c = c ^ 0x20;
        }

        if (write(out, &c, 1) != 1) { err(9, "cant write"); }
    }
    if (read_size < 0) { err(11, "cant read"); }

    close(pfd[0]);
    close(out);

    return 0;
}
