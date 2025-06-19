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
    if (argc != 2) { errx(1, "bad args"); }

    if (mkfifo("/tmp/foo_bar_fifo", 0644) < 0) { err(2, "cant mkfifo"); }

    int fd = open("/tmp/foo_bar_fifo", O_WRONLY);
    if (fd < 0) { err(5, "cant open"); }
    
    if (dup2(fd, 1) < 0) { err(3, "cant dup"); }
    close(fd);

    execlp("cat", "cat", argv[1], (char*)NULL);
    err(4, "cant exec");

    return 0;
}
