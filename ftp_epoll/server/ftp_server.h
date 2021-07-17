#ifndef __FTP_H__
#define __FTP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <syslog.h>
#include <shadow.h>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <dirent.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <ctype.h>

#define FILENAME "file"

typedef void *(*threadfunc)(void *);

typedef struct
{
    int len;
    char buf[1000];
} train, *ptrain;

typedef struct tag_node
{
    int new_fd;
    int sfd;
    char path[128];
    char rpath[128];//根目录
    struct tag_node *pnext;
} Node_t, *pNode_t;

typedef struct
{
    pNode_t phead, ptail;
    int capability;
    int size;
    pthread_mutex_t mutex;
} que_t, *pque;

typedef struct
{
    pthread_t *pth;
    int pth_num;
    pthread_cond_t cond;
    que_t queue;//放描述符的队列
    threadfunc pfunc;
    short startflg;
} factory, *pfac;

void factory_init(pfac p, int pth_num, int cap, threadfunc pfunc);

void factory_start(pfac p);

int handle(pNode_t, char *, int);

int rm_file(char *path, char *buf);

void help(void);

void error_quit(char *msg);

char pt(int type);

int sendls(int new_fd, char *path);

void que_insert(pque p, pNode_t pnew);

void que_get(pque p, pNode_t *pcur);

int recv_n(int new_fd, char *buf, int len);

int filename(char *buf);

int ls_any(pNode_t pcur, char *buf);

int recv_file(int new_fd, char *path);

int send_file(int sfd, char *path, char *buf);

void login_syslog(char *usrname, time_t t);

void cmd_syslog(char *cmd, time_t t);

int send_cd(char *buf, pNode_t pcur);

int display_progress(int progress, int last_char_count);

#endif
