#include "common.h"
#include <strings.h>

int main(int argc, char const *argv[])
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    uint8_t buff[MAXLINE + 1];
    uint8_t recvline[MAXLINE + 1];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0) < 0)) // socket created //0 for tcp
        err_n_die("Error while creating the socket : ");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
        err_n_die("Bind error.");

    if (listen(listenfd, 10) < 0)
        err_n_die("Listen Error");

    while (1)
    {
        struct sockaddr_in addr;
        socklen_t addr_len;

        printf("waiting for a commection in PORT : %d", SERVER_PORT);
        fflush(stdout);

        connfd = accept(listenfd, (SA *)NULL, NULL);

        memset(recvline, 0, MAXLINE);

        while ((n = read(connfd, recvline, MAXLINE - 1) > 0))
        {
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

            if (recvline[n - 1] == '\n')
                break;
            memset(recvline, 0, MAXLINE);
        }

        if(n<0)
            err_n_die("Read error !");

        
        snprintf((char*)buff, sizeof(buff),"HTTP/1.0 200 OK\r\n\r\nHello");




        write(connfd,(char*)buff,strlen((char*)buff));
        close(connfd);
    }

    return 0;
}




char *bin2hex(const unsigned char *input, size_t len)
{
    char *result;
    char *hexits = "0123456789ABCDEF";

    if (input == NULL || len <= 0)
        return NULL;

    int resultLength = (len*3)+1;

    result = malloc(resultLength);
    bzero(result,resultLength);

    for (int i = 0; i < len; i++)
    {
        result[i*3] = hexits[input[i]>>4];
        result[(i*3)+1] = hexits[input[i]& 0x0f];
        result[(i*3)+2] = ' ';
    }
    return result;
}

// error handling
void err_n_die(const char *fmt, ...)
{
    int errno_save;
    va_list ap;

    // save the error
    errno_save = errno;

    // pint
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    // print the error message
    if (errno_save != 0)
    {
        fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);

    _exit(1);
}


