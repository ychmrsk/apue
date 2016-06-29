/* original header file
   Include this file before standard header file.
*/

#ifndef _APUE_H
#define _APUE_H

#define _POSIX_C_SOURCE 200809L

#if defined(SOLARIS)  /* Solaris 10 */
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 700
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/termios.h>  /* for winsize */
#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>  /* for offsetof */
#include <string.h>
#include <unistd.h>
#include <signal.h>  /* for SIG_ERR */

#define MAXLINE 4096

/* Default Access Permission of New File */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* Default Access Permission of New Directory */
#define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void Sigfunc(int);  /* for signal handler */

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/* prototype for original functions */
char *path_alloc(size_t *);  /* fig.2.16 */
long open_max(void);         /* fig.2.17 */

int set_cloexec(int);    /* fig.13.9 */
void clr_fl(int, int);
void set_fl(int, int);   /* fig.3.12 */

void pr_exit(int);     /* fig.8.5 */

void pr_mask(const char *);             /* fig.10.14 */
Sigfunc *signal_intr(int, Sigfunc *);   /* fig.10.19 */

void daemonize(const char *);     /* fig.13.1 */

void sleep_us(unsigned int);                /* prac.14.5 */
ssize_t readn(int, void *, size_t);         /* fig.14.24 */
ssize_t writen(int, const void *, size_t);  /* fig.14.24 */

int fd_pipe(int *);                                             /* fig.17.2 */
int recv_fd(int, ssize_t (*func)(int, const void *, size_t));   /* fig.17.14 */
int send_fd(int, int);                                          /* fig.17.13 */
int send_err(int, int, const char *);                           /* fig.17.12 */
int serv_listen(const char *);                                  /* fig.17.8 */
int serv_accept(int, uid_t *);                                  /* fig.17.9 */
int cli_conn(const char *);                                     /* fig.17.10 */
int buf_args(char *, int (*func)(int, char **));                /* fig.17.23 */

int tty_cbreak(int);                /* fig.18.20 */
int tty_raw(int);                   /* fig.18.20 */
int tty_reset(int);                 /* fig.18.20 */
void tty_atexit(void);              /* fig.18.20 */
struct termios *tty_termios(void);  /* fig.18.20 */

int ptym_open(char *, int);  /* fig.19.9 */
int ptys_open(char *);       /* fig.19.9 */
#ifdef TIOCGWINSZ
/* fig.19.10 */
pid_t pty_fork(int *, char *, int, const struct termios *, const struct winsize *);
#endif

int lock_reg(int, int, int, off_t, int, off_t);  /* fig.14.5 */

#define read_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
  lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

pid_t lock_test(int, int, off_t ,int, off_t);  /* fig.14.6 */

#define is_read_lockable(fd, offset, whence, len) \
  (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
  (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

/* appendix B */
void err_msg(const char *, ...);
void err_dump(const char *, ...) __attribute__((noreturn));
void err_quit(const char *, ...) __attribute__((noreturn));
void err_cont(int, const char *, ...);
void err_exit(int, const char *, ...) __attribute__((noreturn));
void err_ret(const char *, ...);
void err_sys(const char *, ...) __attribute__((noreturn));

/* appendix B */
void log_msg(const char *, ...);
void log_open(const char *, int, int);
void log_quit(const char *, ...) __attribute__((noreturn));
void log_ret(const char *, ...);
void log_sys(const char *, ...) __attribute__((noreturn));
void log_exit(int, const char *, ...) __attribute__((noreturn));

/* chap.8.9 */
void TELL_WAIT(void);
void TELL_PARENT(pid_t);
void TELL_CHILD(pid_t);
void WAIT_PARENT(void);
void WAIT_CHILD(void);

#endif  /* APUE_H */
