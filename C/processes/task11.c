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
    int good = 0;
    int bad = 0;
    int* ptrgood = &good;
    int* ptrbad = &bad;
    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();
        if (pid < 0) { err(1, "cant fork"); }

        if (pid == 0) {
            execlp(argv[i], argv[i], (char*)NULL);
        }
        else {
            int status;
            wait(&status);

            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { (*ptrbad)++; }
            else { (*ptrgood)++; }
        }
    }

    char text[35];
    snprintf(text, 35, "Good: %d, Bad: %d\n", good, bad);
    int len = strlen(text);
    if (write(1, text, len) != len) { err(2, "cant write"); }

    return 0;
}
