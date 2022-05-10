// header file
#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <strings.h>
#include<windows.h>

#define PORT 5000
#define MAXLINE 4096
#define BACKLOCK 5
#define SA struct sockaddr

fd_set fr;
void err_n_die(const char *fmt, ...);
char *bin2hex(const unsigned char *input, size_t len);

// global variable
int nSocket;
struct sockaddr_in srv;

int nArrClient[10];

int main()
{
    int listenfd, connfd, n,nRet;
    char *buff[MAXLINE + 1] = {
        0,
    };
    char *recvline[MAXLINE + 1] = {
        0,
    };

    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
        err_n_die("WSA failed to initialised .\n");
    else
        printf("WSA initialised successfully !\n");

    // initialised the socket
    if ((nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        err_n_die("SOcket is Not Open.");
        WSACleanup();
    }
    else
        printf("Socket is Open at %d\n",nSocket);

// initialised the environment for sockeaddr structure
    // bzero(&srv, sizeof(srv));
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	// srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(srv.sin_zero), 0, 8);


	// bind the socket to the local port
	if ((nRet = bind(nSocket, (SA *)&srv, sizeof(srv))) < 0){
		err_n_die("BIND ERROR .");;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("Successfully bind the local PORT\n");

	// LIsten the request from client (queues the requests)
	if ((nRet = listen(nSocket, BACKLOCK)) < 0){
		err_n_die("LISTEN ERROR .");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("Started listening to local port\n" );


for( ; ; )
    {

        FD_ZERO(&fr);

		FD_SET(nSocket, &fr);

		for (int i = 0; i < 10; i++)
		{
			if (nArrClient[i] != 0)
			{
				FD_SET(nArrClient[i], &fr);
			}
		}

        struct sockaddr_in addr;
        unsigned int addr_len;

        printf("waiting for a commection in PORT : %d\n", PORT);
        fflush(stdout);

        connfd = accept(nSocket, (SA *)NULL, NULL);

        
	int nRet = recv(nArrClient[0], buff, 4096, 0);
           // fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);
           printf("\n %s \n",buff);


            memset(recvline, 0, MAXLINE);
        

        
        // int nLen = sizeof(struct sockaddr);
        // int nClientSocket = accept(nSocket, NULL, &nLen);
        // int nRet = recv(nClientSocket, buff, 256, 0);

        snprintf((char*)buff, sizeof(buff),"HTTP/1.1 200 OK\r\n\r\nHello");
        
        



        write(connfd,(char*)buff,strlen((char*)buff));
        close(connfd);
        //Sleep(2000);
    }

    return 0;
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


// char *bin2hex(const unsigned char *input, size_t len)
// {
//     char *result;
//     char *hexits = "0123456789ABCDEF";

//     if (input == NULL || len <= 0)
//         return NULL;

//     int resultLength = (len*3)+1;

//     result = malloc(resultLength);
//     //bzero(result,resultLength);

//     for (int i = 0; i < len; i++)
//     {
//         result[i*3] = hexits[input[i]>>4];
//         result[(i*3)+1] = hexits[input[i]& 0x0f];
//         result[(i*3)+2] = ' ';
//     }
//     return result;
// }