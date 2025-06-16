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

typedef struct {
    uint16_t start;
    uint8_t f1byte;
    uint8_t f2byte;
} info;

int main(int argc, char* argv[])
{
    if (argc != 4) { errx(1, "bad args"); }

    int f1 = open(argv[1], O_RDONLY);
    if (f1 < 0) { err(2, "cant open"); }
    int f2 = open(argv[2], O_RDONLY);
    if (f2 < 0) { err(3, "cant open"); }
    int patchfd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (patchfd < 0) { err(4, "cant open"); }

    uint8_t c1, c2;
    uint16_t pos = 0;
    info i;
    int r1, r2;
    while((r1 = read(f1, &c1, 1)) > 0 && (r2 = read(f2, &c2, 1)) > 0) {
        if (c1 != c2) {
            i.start = pos;
            i.f1byte = c1;
            i.f2byte = c2;
            if (write(patchfd, &i, sizeof(i)) != sizeof(i)) { err(5, "cant write"); }
        }
        pos++;
    }
    if (r1 < 0) { err(6, "cant read f1"); }
    if (r2 < 0) { err(7, "cant read f2"); }


    close(f1);
    close(f2);
    close(patchfd);

    return 0;
}
