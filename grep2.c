#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_grep(regex_t *pat, FILE *src, int inverse_match);

static struct option longopts[] = {
  {"ignore-case", no_argument, NULL, 'i'},
  {"inverse-match", no_argument, NULL, 'v'},
  {"help", no_argument, NULL, 'h'},
  {0, 0, 0, 0}
};

int
main(int argc, char *argv[])
{
  regex_t pat;
  int err;
  int i;
  int opt;
  int inverse_match = 0;
  int ignore_case = 0;
  int eflag = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;

  while ((opt = getopt_long(argc, argv, "ivh", longopts, NULL)) != -1) {
    switch(opt) {
      case 'i':
        ignore_case = 1;
        break;
      case 'v':
        inverse_match = 1;
        break;
      case 'h':
        fprintf(stdout, "Usage: %s [-i] [-v] [FILE ...]\n", argv[0]);
        exit(0);
      case '?':
        fprintf(stdout, "Usage: %s [-i] [-v] [FILE ...]\n", argv[0]);
        exit(1);
    }
  }

  if (optind == argc) {
    fputs("no pattern\n", stderr);
    exit(1);
  }
  if (ignore_case == 1) {
    eflag = eflag | REG_ICASE;
  }

  err = regcomp(&pat, argv[optind], eflag);
  optind += 1;
  if (err != 0) {
    char buf[1024];

    regerror(err, &pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  }
  if (optind == argc) {
    do_grep(&pat, stdin, inverse_match);
  }
  else {
    for(i = optind; i < argc; i++) {
      FILE *f;
      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_grep(&pat, f, inverse_match);
      fclose(f);
    }
  }

  regfree(&pat);
  exit(0);
}

static void
do_grep(regex_t *pat, FILE *src, int inverse_match)
{
  char buf[4096];

  while (fgets(buf, sizeof buf, src)) {
    if (inverse_match == 0) {
      if (regexec(pat, buf, 0, NULL, 0) == 0) {
        fputs(buf, stdout);
      }
    } else {
      if (regexec(pat, buf, 0, NULL, 0) != 0) {
        fputs(buf, stdout);
      }
    }
  }
}
