#include <stdio.h>
#include <stdlib.h>

static void do_wc_l(FILE *f);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    do_wc_l(stdin);
  } else {
    int i;

    for (i = 1; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_wc_l(f);
      fclose(f);
    }
  }
  exit(0);
}

static void
do_wc_l(FILE *f)
{
  unsigned long count = 0;
  int buffer;
  int prev = '\n';

  while ((buffer = fgetc(f)) != EOF) {
    if (buffer == '\n') { count++; }
    prev = buffer;
  }

  if (prev != '\n') {
    count++; // missing \n line
  }

  printf("%lu\n", count);
}
