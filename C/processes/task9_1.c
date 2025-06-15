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

int main(void)
{
    int fd = open("my_temp_file", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd < 0) { err(3, "cant open"); }

    pid_t pid = fork();
    if (pid < 0) { err(1, "cant fork"); }

    int count = 0;
    int* ptr = &count;

    if (pid == 0) {
        char text[] = "foo";
        for ( ; (*ptr) < 15; (*ptr)++) {
            if (write(1, text, sizeof(text)) != sizeof(text)) { err(4, "cant write"); }
        }
    }
    else {
        char text[] = "bar";
        for ( ; (*ptr) < 15; (*ptr)++) {
            if (write(1, text, sizeof(text)) != sizeof(text)) { err(5, "cant write"); }
        }
    }

    close(fd);
    
    return 0;
}
