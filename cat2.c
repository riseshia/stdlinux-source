#include <stdio.h>
#include <stdlib.h>

static void do_cat(const char *path);

int
main(int argc, char *argv[])
{
  int i;

  if (argc < 2) {
    fprintf(stderr, "%s: file name not given\n", argv[0]);
    exit(1);
  }
  for (i = 1; i < argc; i++) {
    do_cat(argv[i]);
  }
  exit(0);
}

static void
do_cat(const char *path)
{
  FILE *f;
  int c;

  f = fopen(path, "r");
  if (!f) {
    perror(path);
    exit(1);
  }
  while ((c = fgetc(f)) != EOF) {
    if (putchar(c) < 0) exit(1);
  }
  fclose(f);
}
