// bannat v putty chast 1

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
  if (argc != 3) { errx(1, "bad args"); }

  if (strlen(argv[1]) != 1 || !isdigit(argv[1][0]) || strlen(argv[2]) != 1 || !isdigit(argv[2][0])) { errx(2, "bad args"); }

  int p2c[2], c2p[2];
  if (pipe(p2c) < 0) { err(3, "cant pipe"); }
  if (pipe(c2p) < 0) { err(31, "cant pipe"); }
  
  uint8_t N = argv[1][0] - '0', D = argv[2][0] - '0';
  const char *DING = "DING", *DONG = "DONG";
  int len = 4;
  pid_t pid = fork();
  if (pid < 0) { err(4, "cant fork"); }

  if (pid == 0) {
    close(p2c[1]);
    close(c2p[0]);
    
    char buf[5];
    for (int i = 0; i < N; i++) {
      if (read(p2c[0], buf, 4) != 4) { err(10, "cant read"); }
      if (write(1, DONG, 4) != 4) { err(8, "cant write"); }
      if (write(c2p[1], DONG, 4) != 4) { err(9, "cant write"); }
    }

    close(p2c[0]);
    close(c2p[1]);
    exit(0);
  }

  close(p2c[0]);
  close(c2p[1]);
  
  char buf[5];
  for (int i = 0; i < N; i++) {
      if (write(1, DING, 4) != 4) { err(8, "cant write"); }
      if (write(p2c[1], DING, 4) != 4) { err(9, "cant write"); }
      if (read(c2p[0], buf, 4) != 4) { err(10, "cant read"); }
      sleep(D);
  }

  close(p2c[1]);
  close(c2p[0]);

  wait(0);
  return 0;
}
