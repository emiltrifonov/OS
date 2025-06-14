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
    if (argc != 2) { errx(1, "args pls"); }

    pid_t pid = fork();
    if (pid < 0) { err(2, "cant fork"); }

    if (pid == 0) {
        int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if (fd < 0) { err(3, "cant open"); }

        char str[] = "foobar";
        if (write(fd, str, sizeof(str)) != sizeof(str)) {
            err(4, "cant write");
        }

        close(fd);
    }
    else {
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { err(6, "rip child"); }

        int fd = open(argv[1], O_RDONLY);
        if (fd < 0) { err(5, "cant open"); }

        char buf[2];
        char txt[2];
        txt[1] = ' ';
        int read_size;
        while((read_size = read(fd, buf, 2)) > 0) {
            if (read_size == 1) {
                if(write(1, buf, 1) != 1) { err(7, "cant write"); }
                break;
            }
            else {
                txt[0] = buf[0];
                if(write(1, txt, 2) != 2) { err(8, "cant write"); }
            }

            int pos = lseek(fd, -1, SEEK_CUR);
            if (pos < 0) { err(10, "cant lseek"); }
        }

        if (write(1, "\n", 1) != 1) { err(9, "cant write"); }
    }

    return 0;
}
