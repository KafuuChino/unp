#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void *arg);   //线程入口函数
void send_data(FILE *fp, char *ct, char *file_name); //向浏览器客服端发送数据
char *content_type(char *file); //数据类型
void send_error(FILE *fp);  //发送错误处理数据
void error_handling(char *message); //控制台错误打印

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE];
    pthread_t t_id;

    if (argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while (1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_adr, &clnt_adr_sz);
        printf("Connection Request: %s : %d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));//连接的客服端IP与端口

        //多线程
        pthread_create(&t_id, NULL, request_handler, (void *) &clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}

//客服端请求消息处理
void *request_handler(void *arg)
{
    int clnt_sock = *((int *) arg);
    char req_line[SMALL_BUF];
    FILE *clnt_read;
    FILE *clnt_write;

    char method[10];
    char ct[15];
    char file_name[30];

    /*将套接字转换为标准I/O操作*/
    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(req_line, SMALL_BUF, clnt_read);//保存请求行数据
    if (strstr(req_line, "HTTP/") == NULL) //查看是否为HTTP提出的请求
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    strcpy(method, strtok(req_line, " /"));  //请求方式
    strcpy(file_name, strtok(NULL, " /"));  //请求的文件名
    strcpy(ct, content_type(file_name));   //请求内容类型
    if (strcmp(method, "GET") != 0)  //是否为GET请求
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    fclose(clnt_read);
    send_data(clnt_write, ct, file_name); //响应给客服端
    return NULL;
}

//服务端响应消息
void send_data(FILE *fp, char *ct, char *file_name)
{
    char protocol[] = "HTTP/1.0 200 OK\r\n";  //状态行(用HTTP1.1版本进行响应，你的请求已经正确处理)
    char server[] = "Server: Linux Web Server \r\n"; //服务端名
    char cnt_len[] = "Content-length: 2048\r\n";  //数据长度不超过2048
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type, "Content-type: %s\r\n\r\n", ct);
    send_file = fopen(file_name, "r"); //读本地配置文件
    if (send_file == NULL)
    {
        send_error(fp);
        return;
    }

    /*传输头信息*/
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    /*传输请求数据*/
    while (fgets(buf, BUF_SIZE, send_file) != NULL)
    {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);   //服务端响应客服端请求后立即断开连接（短链接）
}

//请求数据的类型
char *content_type(char *file)
{
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
        return "text/html";  //html格式的文本数据
    else
        return "text/plain";
}

//发送客服端错误处理
void send_error(FILE *fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";  //请求文件不存在
    char server[] = "Server: Linux Web Server \r\n";
    char cnt_len[] = "Content-length: 2048\r\n";
    char cnt_type[] = "Content-type: text/html\r\n\r\n";
    char content[] = "发生错误！查看请求文件名和请求方式！";

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
    fclose(fp);
}

//控制台错误打印
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}