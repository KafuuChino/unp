//
// Created by yuanh on 2021/5/7.
//
#include <sys/socket.h>
#include <error.h>
#include <errno.h>
#include <err.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cctype>

#define SERV_PORT 8888
#define BUFFSIZE 1024
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    int ret;
    char buf[BUFFSIZE];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_len;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd==-1)
    {
        sys_err("socket error");
    }
    int bind_fd = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bind_fd==-1)
    {
        sys_err("bind error");
    }
    listen(lfd, 128);

    client_addr_len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (cfd==-1)
    {
        sys_err("accept error");
    }
    while (1)
    {
        ret = read(cfd, buf, sizeof(buf));
        if (ret==0)
        {
            break;
        }
        write(STDOUT_FILENO, buf, ret);
        for (int i = 0; i < ret; ++i)
        {
            toupper(buf[i]);
        }
        write(cfd, buf, ret);
    }
    close(lfd);
    close(cfd);
    return 0;
}
