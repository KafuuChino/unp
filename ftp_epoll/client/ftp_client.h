#ifndef     __FTP_EPOLL_CLIENT_FUN_H__
#define     __FTP_EPOLL_CLIENT_FUN_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define FILENAME "file"
typedef struct data
{
    pid_t pid;
    int sfd;
    short busy;
} data_t, *pdata;

typedef struct
{
    int len;
    char buf[1000];
} train, *ptrain;

void make_child(pdata, int);

int send_n(int, char *, int);

int recv_ls(int new_fd);

int recv_file(int new_fd);

int send_file(int sfd, char *buf);

int display_progress(int progress, int last_char_count);


#endif