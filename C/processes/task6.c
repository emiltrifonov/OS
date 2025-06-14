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
    if (pid == 0) {
        //child

        execlp(argv[1], argv[1], (char*)NULL);
    }
    else {
        int status;
        wait(&status);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            char text[] = "Command: ";
            write(1, text, strlen(text));
            write(1, argv[1], strlen(argv[1]));
            write(1, "\n", 1);
        }
    }

    return 0;
}
