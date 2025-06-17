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

void delete(char*);
void squeeze(char*);
void translate(char*, char*);

void delete(char* arg) {
    char arr[256];
    int len = strlen(arg);
    for (int i = 0; i < len; i++) {
        arr[arg[i]+128]=1;
    }

    char byte;
    int read_size;
    while((read_size = read(0, &byte, sizeof(byte))) > 0) {
        if (arr[byte+128] == 0) {
            if (write(1, &byte, sizeof(byte)) != sizeof(byte)) { err(2, "cant write"); }
        }
    }
    if (read_size < 0) { err(3, "cant read"); }
}

void squeeze(char* arg) {
    char arr[256];
    int len = strlen(arg);
    for (int i = 0; i < len; i++) {
        arr[arg[i]+128] = 1;
    }

    char byte;
    int16_t prev = INT16_MIN;
    int read_size;
    while((read_size = read(0, &byte, sizeof(byte))) > 0) {
        if (byte != prev || arr[byte+128] == 0) {
            if (write(1, &byte, sizeof(byte)) != sizeof(byte)) { err(4, "cant write"); }
        }

        prev = byte;
    }
    if (read_size < 0) { err(5, "cant read"); }
}

void translate(char* arg1, char* arg2) {
    int len = strlen(arg1);
    if (len != strlen(arg2)) { err(6, "diff len sets"); }
    int arr[256];
    for (int i = 0; i < 256; i++) { arr[i] = -1; }
    for (int i = 0; i < len; i++) {
        // map char to index
        arr[arg1[i]+128] = i;
    }

    char byte;
    int read_size;
    while((read_size = read(0, &byte, sizeof(byte))) > 0) {
        if (arr[byte+128] != -1) {
            byte = arg2[arr[byte+128]];
        }
        if (write(1, &byte, sizeof(byte)) != sizeof(byte)) { err(7, "cant write"); }
    }
    if (read_size < 0) { err(8, "cant read"); }
}

int main(int argc, char* argv[])
{
    if (argc != 3) { errx(1, "bad args"); }
    if (strcmp(argv[1], "-d") == 0) {
        delete(argv[2]);
    }
    else if (strcmp(argv[1], "-s") == 0) {
        squeeze(argv[2]);
    }
    else {
        translate(argv[1], argv[2]);
    }
    return 0;
}
