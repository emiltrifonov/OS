// bannat v putty chast 2

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
#include <errno.h>
#include <limits.h>

const char* L[] = { "tic", "tac", "toe\n" };
char filler = "\0";

void loop(int in, int out, int counter, int index, int increment, int max_words) {
  char buf;
  int read_size;
  
  while((read_size = read(in, &buf, 1)) > 0) {
    if (counter > max_words) {
      close(in);
      close(out);
      exit(0);
    }
    
    if (write(1, L[index], strlen(L[index])) != strlen(L[index])) ( err(7, "cant write"); }

    counter += increment;
    index = (index + increment) % 3;
    
    if (write(out, &filler, 1) != 1) ( err(8, "cant write"); }    
  }
  if (read_size < 0) { err(9, "cant read"); }
}

int main(int argc, char *argv[])
{
  if (argc != 3) { errx(1, "bad args"); }

  int NC = strtol(argv[1], NULL, 10);
  int WC = strtol(argv[2], NULL, 10);

  int pfd[7][2];
  for (int i = 0; i < NC; i++) {
    if (pipe(pfd[i]) < 0) { err(4, "cant pipe"); }
  }

  for (int i = 0; i < NC; i++) {
    int in = pfd[i][0];
    int out = (i == NC - 1) ? pfd[0][1] : pfd[i+1][1];

    pid_t pid = fork();
    if (pid < 0) { err(5, "cant fork"); }

    if (pid == 0) {
      for (int j = 0; j < NC; j++) {
        if (pfd[j][0] != in) { close(pfd[j][0]); }
        if (pfd[j][1] != out) { close(pfd[j][1]); }
      }
  
      loop(in, out, 2, (j + 1) % 3, NC, WC);
  
      return 0;
    }
  }

  for (int j = 0; j < NC; j++) {
      close(pfd[j][0]);
      if (j != 0) { close(pfd[j][1]); }
  }

  if (write(pfd[0][1], L[0], 3) != 3) { err(6, "cant write"); }
  close(pfd[0][1]);
  
  for (int j = 0; j < NC; j++) {
      wait(0);
  }
  
  return 0;
}
