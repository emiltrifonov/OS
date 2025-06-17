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
    uint32_t uid;
    uint16_t x1;
    uint16_t x2;
    uint32_t session_start;
    uint32_t session_end;
} record;

typedef struct {
    uint32_t uid;
    double longest_session;
    uint8_t qualified;
} user_info;

user_info* getUserInfoByUid(user_info* users, ssize_t count, uint32_t uid) {
    user_info* res = NULL;
    for (ssize_t i = 0; i < count; i++) {
        if (users[i].uid == uid) {
            res = &users[i];
            break;
        }
    }
    return res;
}

double getDispersion(record* records, ssize_t count) {
    double avg = 0.0;
    for (ssize_t i = 0; i < count; i++) {
        avg += (double)records[i].session_end - (double)records[i].session_start;
    }
    avg /= (double)count;
    double disp = 0.0;
    for (ssize_t i = 0; i < count; i++) {
        double temp = (double)records[i].session_end - (double)records[i].session_start - avg;
        disp += (temp * temp);
    }
    disp /= (double)count;
    return disp;
}

int main(int argc, char* argv[])
{
    if (argc != 2) { errx(1, "bad args"); }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { err(2, "cant open"); }

    ssize_t records_size = 16384;
    record records[records_size];
    ssize_t users_size = 2048;
    user_info users[users_size];
    uint16_t users_count = 0;

    struct stat st;
    if (fstat(fd, &st) < 0) { err(3, "cant fstat"); }
    ssize_t records_count = st.st_size / sizeof(record);

    if (sizeof(record) * records_count != st.st_size) { err(4, "bad format"); }

    if (read(fd, records, sizeof(record) * records_count) != sizeof(record) * records_count) { err(5, "cant read"); }

    double dispersion = getDispersion(records, records_count);

    for (ssize_t i = 0; i < records_count; i++) {
        uint32_t uid = records[i].uid;
        double duration = (double)records[i].session_end - (double)records[i].session_start;

        user_info* user = getUserInfoByUid(users, users_count, uid);
        if (!user) {
            users[users_count].uid = uid;
            users[users_count].longest_session = duration;
            users[users_count].qualified = 0;
            users_count++;
        }
        else if ((*user).longest_session < duration) {
            (*user).longest_session = duration;
        }

        if (user && duration * duration > dispersion) {
            (*user).qualified = 1;
        }
    }

    char text[128];
    for (ssize_t i = 0; i < users_count; i++) {
        if (users[i].qualified) {
            int len = snprintf(text, 128, "%u %f\n", users[i].uid, users[i].longest_session);
            if (write(1, text, len) != len) { err(6, "cant write"); }
        }
    }

    return 0;
}
