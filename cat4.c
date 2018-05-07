#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_cat(FILE *f, int visibleFlg);

static struct option longopts[] = {
  {"visible", no_argument, NULL, 'b'},
  {0, 0, 0, 0}
};

int
main(int argc, char *argv[])
{
  int opt;
  int visibleFlg = 0;

  while ((opt = getopt_long(argc, argv, "b", longopts, NULL)) != -1) {
    switch(opt) {
      case 'b':
        visibleFlg = 1;
        break;
      case '?':
        fprintf(stdout, "Usage: %s [-b] [FILE ...]\n", argv[0]);
        exit(1);
    }
  }

  if (optind == argc) {
    do_cat(stdin, visibleFlg);
  } else {
    int i;

    for (i = optind; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_cat(f, visibleFlg);
      fclose(f);
    }
  }
  exit(0);
}

static void
do_cat(FILE *f, int visibleFlg)
{
  int c;

  if (visibleFlg == 1) {
    while ((c = fgetc(f)) != EOF) {
      switch (c) {
        case '\t':
          if (fputs("\\t", stdout) == EOF) exit(1);
          break;
        case '\n':
          if (fputs("$\n", stdout) == EOF) exit(1);
          break;
        default:
          if (putchar(c) < 0) exit(1);
          break;
      }
    }
  } else {
    while ((c = fgetc(f)) != EOF) {
      if (putchar(c) < 0) exit(1);
    }
  }
}
