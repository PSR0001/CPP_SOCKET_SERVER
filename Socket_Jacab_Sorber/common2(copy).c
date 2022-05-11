// header file
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
//#include<winsock.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<ws2tcpip.h>


// define macros
#define SERVER_PORT 8000
#define MAXLINE 4096
#define SA struct sockaddr

// function
// handel error
void err_n_die(const char *fmt, ...);

int inet_pton(int af, const char *src, void *dst);




int main(int argc, char const *argv[])
{
    int sockfd, n;
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    if (argc != 2)
        err_n_die("usage : %s <server address>", argv[0]);
    
    WSADATA ws;

    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
         printf( "WSA failed to initialised .\n");
    else
      printf("WSA initialised successfully !\n");
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) < 0)) // socket created //0 for tcp
        err_n_die("Error while creating the socket : \n");

     printf("\n%d\n",sockfd);
    ZeroMemory(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    int OP;
    OP=inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if (OP <= 0)
        err_n_die("inet_pton error for %s", argv[1]);
   
         // translate address //  "1.2.3.4" => [1,2,3,4]
//        printf("\n%s\n\n",argv[1]);
//  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
        printf("\n%d\n",sockfd);
    
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
        err_n_die("\nConnection Failed\n");

    sprintf(sendline, "GET/HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);

    // send the request

    if (write(sockfd, sendline, sendbytes) != sendbytes)
        err_n_die("write error");


    memset(recvline,0,MAXLINE);

    //read the server response
    while((n=read(sockfd,recvline,MAXLINE-1)>0)){
        printf("%s",recvline);

    }
    if(n<0)
      err_n_die("read error");


   // _exit(0); //end successfully

return 0;



  //  return 0;
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



int inet_pton(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
    return 1;
      case AF_INET6:
    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
    return 1;
    }
  }
  return 0;
}