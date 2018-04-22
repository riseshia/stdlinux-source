#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_cat(int fd, const char *path);
static void die(const char *s);

int
main(int argc, char *argv[])
{
  int i;
  int fd;

  if (argc < 2) {
    do_cat(STDOUT_FILENO, "stdin");
  } else {
    for (i = 1; i < argc; i++) {
      fd = open(argv[i], O_RDONLY);
      if (fd < 0) die(argv[i]);
      do_cat(fd, argv[i]);
      if (close(fd) < 0) die(argv[i]);
    }
  }

  exit(0);
}

#define BUFFER_SIZE 2048

static void
do_cat(int fd, const char *path)
{
  unsigned char buf[BUFFER_SIZE];
  int n;

  for (;;) {
    n = read(fd, buf, sizeof buf);
    if (n < 0) die(path);
    if (n == 0) break;
    if (write(STDOUT_FILENO, buf, n) < 0) die(path);
  }
}

static void
die(const char *s)
{
  perror(s);
  exit(1);
}
