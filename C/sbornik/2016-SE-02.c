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
    uint32_t start;
    uint32_t length;
} pair;

int main(void)
{
    int f1 = open("f1", O_RDONLY);
    if (f1 < 0) { err(1, "cant open"); }
    int f2 = open("f2", O_RDONLY);
    if (f2 < 0) { err(2, "cant open"); }
    int f3 = open("f3", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (f3 < 0) { err(3, "cant open"); }

    pair p;
    int read_size;
    uint32_t num;
    while((read_size = read(f1, &p, sizeof(p))) == sizeof(p)) {
        if (lseek(f2, p.start * sizeof(uint32_t), SEEK_SET) < 0) { err(5, "cant repos"); }
        for (uint32_t i = 0; i < p.length; i++) {
            if (read(f2, &num, sizeof(num)) != sizeof(num)) { err(6, "cant read"); }
            if (write(f3, &num, sizeof(num)) != sizeof(num)) { err(7, "cant write"); }
        }
    }
    if (read_size < 0) { err(4, "cant read"); }

    close(f1);
    close(f2);
    close(f3);

    return 0;
}
