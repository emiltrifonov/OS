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
    if (argc != 4) { errx(1, "bad args"); }

    int fd = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (fd < 0) { err(3, "cant open"); }

    pid_t pid = fork();
    if (pid < 0) { err(2, "cant fork"); }

    if (pid == 0) {
        execlp(argv[1], argv[1], (char*)NULL);
    }
    else {
        int status;
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            char txt[] = "first command failed\n";
            if (write(1, txt, sizeof(txt)) != sizeof(txt)) { err(6, "cant write"); }
        }
        else {
            int len = strlen(argv[0]);
            if (write(fd, argv[0], len) != len) { err(5, "cant write"); }
        }

        execlp(argv[2], argv[2], (char*)NULL);
    }

    return 0;
}
