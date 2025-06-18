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

#define true 1

int main(void)
{
    char prompt[] = "Waiting for command: ";
    while(true) {
        if (write(1, prompt, sizeof(prompt)) != sizeof(prompt)) { err(1, "cant write"); }
        char cmd[128];
        if (read(0, cmd, 128) < 0) { err(2, "cant read"); }

        int len = strlen(cmd);
        cmd[len - 1] = '\0';

        if (strcmp("exit", cmd) == 0) { exit(0); }

        pid_t pid = fork();
        if (pid < 0) { err(3, "cant fork"); }

        if (pid == 0) {
            char path[4096];
            snprintf(path, 4096, "/bin/%s", cmd);
            execl(path, cmd, (char*)NULL);
            err(4, "cant exec");
        }

        wait(0);
    }

    return 0;
}
