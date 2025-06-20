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

typedef struct {
    char name[8];
    uint32_t offset;
    uint32_t length;
} data;

int main(int argc, char* argv[])
{
    if (argc != 2) { errx(1, "bad args"); }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { err(2, "cant open"); }

    int pfd[8][2];
    int ind = 0;
    data d;
    int read_size;
    uint16_t ans = 0;

    while ((read_size = read(fd, &d, sizeof(d))) > 0) {
        uint16_t res = 0, num;

        if (pipe(pfd[ind]) < 0) { err(3, "cant pipe"); }
        pid_t pid = fork();
        if (pid < 0) { err(4, "cant fork"); }

        if (pid == 0) {
            int fd2 = open(d.name, O_RDONLY);
            if (fd2 < 0) { err(9, "cant open"); }

            if (lseek(fd2, d.offset * sizeof(uint16_t), SEEK_SET) < 0) { err(10, "cant lseek"); }

            for (int i = 0; i < ind; i++) {
                close(pfd[i][0]);
                close(pfd[i][1]);
            }
            close(pfd[ind][0]);

            for (int i = 0; i < d.length; i++) {
                if (read(fd2, &num, sizeof(num)) != sizeof(num)) { err(5, "cant read"); }
                res = res ^ num;
            }

            if (write(pfd[ind][1], &res, sizeof(res)) != sizeof(res)) { err(6, "cant write"); }
            close(pfd[ind][1]);
            close(fd2);
            exit(0);
        }
        close(pfd[ind][1]);
        ind++;
    }

    for (int i = 0; i < ind; i++) {
        uint16_t num;
        if (read(pfd[i][0], &num, sizeof(num)) != sizeof(num)) { err(7, "cant read"); }
        close(pfd[i][0]);
        ans = ans ^ num;
    }

    char text[32];
    int len = snprintf(text, 32, "result: %04X\n", ans);
    if (write(1, text, len) != len) { err(8, "cant write"); }

    close(fd);
    for (int i = 0; i < ind; i++) { wait(0); }
    return 0;
}
