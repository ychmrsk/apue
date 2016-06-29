/*
            | add str       |  parameter   |         |    
            | from strerror |  to strerror |  exit   |    
 -----------+---------------+--------------+---------|    
 * err_dump |       o       |     errno    | abort() | 
 * err_exit |       o       |     param    | exit(1) | 
 * err_msg  |       x       |       -      | return  | 
 * err_quit |       x       |       -      | exit(1) | 
 * err_ret  |       o       |     errno    | return  | 
 * err_sys  |       o       |     errno    | exit(1) | 
 * err_cont |       o       |     param    | return  | 
 -----------------------------------------------------

 */                          


#include "apue.h"
#include <errno.h>
#include <stdarg.h>  /* variable argument */

static void err_doit(int, int, const char *, va_list);

void err_ret(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);
}

void err_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);
  exit(1);
}

void err_cont(int error, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, error, fmt, ap);
  va_end(ap);
}

void err_exit(int error, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, error, fmt, ap);
  va_end(ap);
  exit(1);
}

void err_dump(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);
  abort();  /* core dump */
  exit(1);
}

void err_msg(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, 0, fmt, ap);
  va_end(ap);
}

void err_quit(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, 0, fmt, ap);
  va_end(ap);
  exit(1);
}

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
  char buf[MAXLINE];

  vsnprintf(buf, MAXLINE-1, fmt, ap);
  if (errnoflag)
    snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s", strerror(error));
  strcat(buf, "\n");
  fflush(stdout);
  fputs(buf, stderr);
  fflush(NULL);
}
