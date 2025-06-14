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
    if (argc != 4) { errx(1, "args pls"); }

    for (int i = 1; i <= 3; i++) {
        pid_t pid = fork();
        if (pid < 0) { err(2, "cant fork"); }

        if (pid == 0) {
            execlp(argv[i], argv[i], (char*)NULL);
        }
        else {
            int status;
            wait(&status);

            char text[24];
            snprintf(text, 24, "%d %d\n", pid, status);
            write(1, text, sizeof(text));
        }
    }

    return 0;
}
