// samo ednata versiq po OBQSNIMI PRICHINI

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
    uint32_t magic;
    uint8_t header_version;
    uint8_t data_version;
    uint16_t count;
    uint32_t reserved1;
    uint32_t reserved2;
} header;

typedef struct {
    uint16_t offset;
    uint8_t original;
    uint8_t new;
} data0;

typedef struct {
    uint32_t offset;
    uint16_t original;
    uint16_t new;
} data1;

void dataVersion0(int pfd, int f1d, int f2d) {
    char buf[4096];
    int read_size;
    while ((read_size = read(f1d, buf, 4096)) > 0) {
        if (write(f2d, buf, read_size) != read_size) { err(6, "cant write"); }
    }
    if (read_size < 0) { err(7, "cant read"); }

    data0 d;
    while((read_size = read(pfd, &d, sizeof(d))) > 0) {
        if (lseek(f1d, d.offset, SEEK_SET) < 0) { err(8, "cant lseek"); }
        if (lseek(f2d, d.offset, SEEK_SET) < 0) { err(9, "cant lseek"); }

        uint8_t c1;
        if (read(f1d, &c1, 1) != 1) { err(10, "cant read"); }
        if (c1 != d.original) { errx(11, "incorrect input file not matching"); }

        if (write(f2d, &d.new, sizeof(d.new)) != sizeof(d.new)) { err(12, "cant write"); }
    }
    if (read_size < 0) { err(13, "cant read"); }
}

void dataVersion1(int pfd, int f1d, int f2d) {

}

int main(int argc, char* argv[])
{
    if (argc != 4) { errx(1, "bad agrs"); }

    int patchfd = open(argv[1], O_RDONLY);
    if (patchfd < 0) { err(2, "cant open"); }
    int f1d = open(argv[2], O_RDONLY);
    if (f1d < 0) { err(3, "cant open"); }
    int f2d = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (f2d < 0) { err(4, "cant open"); }

    header h;
    if (read(patchfd, &h, sizeof(h)) != sizeof(h)) { err(5, "cant read"); }
    if (h.data_version == 0) {
        dataVersion0(patchfd, f1d, f2d);
    }
    else {
        dataVersion1(patchfd, f1d, f2d);
    }

    return 0;
}
