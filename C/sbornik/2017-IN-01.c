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
    uint8_t length;
    uint8_t reserved;
} idx;

int main(int argc, char* argv[])
{
    if (argc != 5) { errx(1, "bad args"); }

    int f1dat = open(argv[1], O_RDONLY);
    if (f1dat < 0) { err(2, "cant open"); }
    int f1idx = open(argv[2], O_RDONLY);
    if (f1idx < 0) { err(3, "cant open"); }
    int f2dat = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (f2dat < 0) { err(4, "cant open"); }
    int f2idx = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (f2idx < 0) { err(5, "cant open"); }

    idx info;
    int read_size;
    uint16_t accsize = 0;
    while ((read_size = read(f1idx, &info, sizeof(info))) == sizeof(info)) {
        if (lseek(f1dat, info.start, SEEK_SET) < 0) { err(6, "cant repos"); }

        const size_t size = UINT8_MAX;
        uint8_t buf[size];
        if (read(f1dat, buf, info.length) != info.length) { err(8, "cant read"); }
        if (buf[0] < 0x41 || buf[0] > 0x5A) { continue; }
        if (write(f2dat, buf, info.length) != info.length) { err(9, "cant write"); }

        info.start = accsize;
        if (write(f2idx, &info, sizeof(info)) != sizeof(info)) { err(10, "cant write"); }
        accsize += info.length;
    }
    if (read_size < 0) { err(11, "cant read"); }
    if (read_size != sizeof(info)) { err(12, "bad format"); }

    close(f1dat); close(f1idx);
    close(f2dat); close(f2idx);
    return 0;
}
