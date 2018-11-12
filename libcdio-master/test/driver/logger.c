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

#include <cdio/logging.h>

char *last_debugmsg = NULL;
char *last_infomsg = NULL;
char *last_warnmsg = NULL;
char *last_errmsg = NULL;
char *last_assertmsg = NULL;
char *last_othermsg = NULL;

/* Here is an example of a custom log handler. */
static void
test_log_handler (cdio_log_level_t level, const char *message)
{
switch(level) {
  case CDIO_LOG_DEBUG:
    last_debugmsg = strdup(message);
    return;
  case CDIO_LOG_INFO:
    last_infomsg = strdup(message);
    return;
  case CDIO_LOG_WARN:
    last_warnmsg = strdup(message);
    return;
  case CDIO_LOG_ERROR:
    last_errmsg = strdup(message);
    return;
  case CDIO_LOG_ASSERT:
    last_assertmsg = strdup(message);
    return;
  default:
    fprintf(stderr, "level %d message: %s should not happen\n",
	   level, message);
    exit(20);
  }
}

int
main(int argc, const char *argv[])
{
    const char *test_msg = "test message";
#ifndef HAVE_WIN32_CDROM
    cdio_log_handler_t old_log_handler = (cdio_log_handler_t) NULL;

    old_log_handler = cdio_log_set_handler(test_log_handler);
    if (old_log_handler != cdio_default_log_handler) {
      fprintf(stderr, "Should have gotten old log handler back %p vs %p\n",
	      (void *) old_log_handler,
	      (void *) cdio_default_log_handler);
	exit(1);
    }
#else
    cdio_log_set_handler(test_log_handler);
#endif

    /* Check that calling each of the logger routine appears in the right variable,
       based on the log level */

    test_msg = "debug";
    cdio_debug("%s", test_msg);

    if (last_debugmsg != NULL) {
	fprintf(stderr, "debug message should have been ignored due to default log level\n");
	exit(2);
    }

    cdio_loglevel_default = CDIO_LOG_DEBUG;
    cdio_debug("%s", test_msg);
    if (last_debugmsg == NULL ||
	strncmp(test_msg, last_debugmsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "debug message %s did not get handled\n",
		last_debugmsg);
	exit(2);
    }

    test_msg = "info";
    cdio_info("%s", test_msg);
    if (strncmp(test_msg, last_infomsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "info message %s did not get handled\n", last_infomsg);
	exit(3);
    }

    test_msg = "warn";
    cdio_warn("%s", test_msg);
    if (strncmp(test_msg, last_warnmsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "warn message %s did not get handled\n", last_warnmsg);
	exit(4);
    }

    test_msg = "error";
    cdio_error("%s", test_msg);
    if (strncmp(test_msg, last_errmsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "error message %s did not get handled\n", last_errmsg);
	exit(5);
    }

    /* Try using generic cdio_log routine */

    test_msg = "debug via cdio_log";
    cdio_log(CDIO_LOG_DEBUG, "%s", test_msg);
    if (strncmp(test_msg, last_debugmsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "debug message %s did not get handled, phase 2\n", last_debugmsg);
	exit(6);
    }

    test_msg = "info via cdio_log";
    cdio_log(CDIO_LOG_INFO, "%s", test_msg);
    if (strncmp(test_msg, last_infomsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "info message %s did not get handled\n", last_infomsg);
	exit(7);
    }

    test_msg = "warn via cdio_log";
    cdio_log(CDIO_LOG_WARN, "%s", test_msg);
    if (strncmp(test_msg, last_warnmsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "warn message %s did not get handled\n", last_warnmsg);
	exit(8);
    }

    test_msg = "error via cdio_log";
    cdio_log(CDIO_LOG_ERROR, "%s", test_msg);
    if (strncmp(test_msg, last_errmsg, strlen(test_msg)) != 0) {
	fprintf(stderr, "error message %s did not get handled\n", last_errmsg);
	exit(9);
    }

    exit(0);
}
