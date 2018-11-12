#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

char *cdio_abspath(const char *cwd, const char *fname);
char *cdio_dirname(const char *fname);

int
main(int argc, const char *argv[])
{
    const char *cue_filename[] = {
	"/tmp/foo.bar",
	"foo.bar"
    };
    const char *expect[] = {
	"/tmp/baz",
	"./baz"
    };

  int rc=0;
  int i;

  for (i=0; i<2; i++) {
      char *dirname = cdio_dirname(cue_filename[i]);
      char *dest = cdio_abspath(dirname, "baz");
      if (0 != strcmp(expect[i], dest)) {
	  fprintf(stderr, 
		  "Incorrect: expecting %s, got %s.\n",
		  expect[i], dest);
	  rc=i+1;
      }
      free(dirname);
      free(dest);
  }
  exit(rc);
}
