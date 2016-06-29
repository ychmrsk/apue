/*
            | add str       |  parameter   |         |    
            | from strerror |  to strerror |  exit   |    
 -----------+---------------+--------------+---------|    
 * log_msg  |       x       |       -      | return  | 
 * log_quit |       x       |       -      | exit(2) | 
 * log_ret  |       o       |     errno    | return  | 
 * log_sys  |       o       |     errno    | exit(2) | 
 * log_exit |       o       |     param    | exit(2) | 
 -----------------------------------------------------
 */                          

#include "apue.h"
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>

static void log_doit(int, int, int, const char *, va_list ap);

/* calling src must define this */
extern int log_to_stderr;

/* if daemon, init syslog() */
void log_open(const char *ident, int option, int facility)
{
  if (log_to_stderr == 0)
    openlog(ident, option, facility);
}

/* system call -- nonfatal error
   show message and errno, and return */
void log_ret(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  log_doit(1, errno, LOG_ERR, fmt, ap);
  va_end(ap);
}

/* system call -- fatal error
   show message, and exit */
void log_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  log_doit(1, errno, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(2);
}

/* non system call -- nonfatal error
   show message, and return */
void log_msg(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  log_doit(0, 0, LOG_ERR, fmt, ap);
  va_end(ap);
}

/* non system call -- fatal error
   show message, and exit */
void log_quit(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  log_doit(0, 0, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(2);
}

/* non nystem call -- fatal error
   errorcode passed by argument
   show message, and exit */
void log_exit(int error, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  log_doit(1, errno, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(2);
}

/* show message, and return
   calling src directs "errnoflag" and "priority" */
static void log_doit(int errnoflag, int error, int priority, const char *fmt, va_list ap)
{
  char buf[MAXLINE];

  vsnprintf(buf, MAXLINE-1, fmt, ap);
  if (errnoflag)
    snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s", strerror(error));
  strcat(buf, "\n");
  if (log_to_stderr) {
    fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
  } else {
    syslog(priority, "%s", buf);
  }
}
