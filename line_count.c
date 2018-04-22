#include <stdio.h>
#include <stdlib.h>

static void do_count(FILE *s);
static void die(const char *f);

int
main(int argc, char *argv[])
{

  if (argc < 2) {
    do_count(stdin);
  } else {
    int i;

    for (i = 1; i < argc; i++) {
      FILE *f;
      f = fopen(argv[i], "r");
      if (!f) die(argv[i]);
      do_count(f);
      if (fclose(f) < 0) die(argv[i]);
    }
  }

  exit(0);
}

static void
do_count(FILE *f)
{
  int c;
  int count = 1;

  while ((c = fgetc(f)) != EOF) {
    if (c == '\n') { count++; }
  }
  fprintf(stdout, "%d\n", count);
}

static void
die(const char *s)
{
  perror(s);
  exit(1);
}
