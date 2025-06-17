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

int compare(const void* f, const void* s) {
    uint32_t x = *(const uint32_t*)f, y = *(const uint32_t*)s;
    return (x>y) - (x<y);
}

void merge(int f1, int f2, int output) {
    uint32_t n1;
    int r1;
    if ((r1 = read(f1, &n1, 4)) < 0) { err(16, "cant read"); }

    uint32_t n2;
    int r2;
    if ((r2 = read(f2, &n2, 4)) < 0) { err(17, "cant read"); }

    uint8_t has1 = (r1 == 4);
    uint8_t has2 = (r2 == 4);

    while (has1 && has2) {
        if (n1 < n2) {
            if (write(output, &n1, 4) != 4) { err(18, "cant write"); }
            if ((r1 = read(f1, &n1, 4)) < 0) { err(19, "cant read"); }

            has1 = (r1 == 4);
        }
        else {
            if (write(output, &n2, 4) != 4) { err(20, "cant write"); }
            if ((r2 = read(f2, &n2, 4)) < 0) { err(21, "cant write"); }

            has2 = (r2 == 4);
        }
    }

    while(has1) {
        if (write(output, &n1, 4) != 4) { err(22, "cant write"); }
        if ((r1 = read(f1, &n1, 4)) < 0) { err(23, "cant read"); }

        has1 = (r1 == 4);
    }

    while(has2) {
        if (write(output, &n2, 4) != 4) { err(24, "cant write"); }
        if ((r2 = read(f2, &n2, 4)) < 0) { err(25, "cant read"); }

        has2 = (r2 == 4);
   }

}

int main(int argc, char* argv[])
{
    if (argc != 3) { errx(1, "bad args"); }

    int input = open(argv[1], O_RDONLY);
    if (input < 0) { err(2, "cant open"); }
    int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (output < 0) { err(3, "cant open"); }

    struct stat st;
    if (fstat(input, &st) < 0) { err(4, "cant fstat"); }
    ssize_t size = st.st_size;
    ssize_t numcount = size / sizeof(uint32_t);

    const ssize_t maxbuf = 2*1024*1024;
    uint32_t buf[maxbuf];

    if (size < maxbuf) {
        if (read(input, buf, size) != size) { err(5, "cant read"); }
        qsort(buf, numcount, sizeof(uint32_t), compare);
        if (write(output, buf, size) != size) { err(6, "cant write"); }

        close(input);
        close(output);
        return 0;
    }

    char t1[] = "/tmp/sort_XXXXXX";
    char t2[] = "/tmp/sort_XXXXXX";

    int t1fd = mkstemp(t1);
    if (t1fd < 0) { err(7, "cant make temp"); }
    int t2fd = mkstemp(t2);
    if (t2fd < 0) { err(8, "cant make temp"); }

    uint8_t writeToTemp1 = 1;
    while (numcount > 0) {
        ssize_t chunk_size = (numcount < maxbuf) ? numcount : maxbuf;

        if (read(input, buf, chunk_size*4) != chunk_size*4) { err(9, "cant read"); }

        qsort(buf, chunk_size, 4, compare);

        if (writeToTemp1) {
            if (write(t1fd, buf, chunk_size*4) != chunk_size*4) { err(10, "cant write"); }
        }
        else {
            if (write(t2fd, buf, chunk_size*4) != chunk_size*4) { err(11, "cant write"); }
        }

        writeToTemp1 = !writeToTemp1;
        numcount -= chunk_size;
    }

    if (lseek(t1fd, 0, SEEK_SET) < 0) { err(12, "cant lseek f1"); }
    if (lseek(t2fd, 0, SEEK_SET) < 0) { err(13, "cant lseek f2"); }

    merge(t1fd, t2fd, output);

    close(input);
    close(output);

    if (unlink(t1) < 0) { err(14, "cant unlink f1"); }
    if (unlink(t2) < 0) { err(15, "cant unlink f2"); }

    return 0;
}
