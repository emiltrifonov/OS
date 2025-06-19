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

// returns: 1 - for invalid, 0 - for valid
uint8_t execute(char* argv[], time_t mindur, int fd) {
    time_t start_time = time(NULL);

    pid_t pid = fork();
    if (pid < 0) { err(4, "cant fork"); }

    if (pid == 0) {
        execvp(argv[0], argv);
        err(17, "cant exec");
    }

    int status;
    wait(&status);
    time_t end_time = time(NULL);

    int exit_code = 0;

    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status)) {
        exit_code = 129;
    }
    else {
        exit_code = 128;
    }

    char data[4096];
    int len = snprintf(data, 4096, "%ld %ld %d\n", start_time, end_time, exit_code);
    if (write(fd, data, len) != len) { err(6, "cant write"); }

    return (exit_code != 0) && ((end_time - start_time) < mindur);
}

int main(int argc, char* argv[])
{
    if (argc < 3) { errx(1, "bad args"); }
    if (strlen(argv[1]) != 1 || argv[1][0] < '1' || argv[1][0] > '9') { errx(2, "bad minimum duration"); }

    time_t mindur = argv[1][0] - '0';
    uint8_t invalid_count = 0;

    int fd = open("run.log", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd < 0) { err(5, "cant open"); }

    while (invalid_count < 2) {
        if(!execute(argv+2, mindur, fd)) {
            invalid_count = 0;
        }
        else {
            invalid_count++;
        }
    }

    close(fd);

    return 0;
}
