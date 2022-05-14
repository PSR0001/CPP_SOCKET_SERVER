#include "client.h"

// Global Variable
int sockfd, n, nRet;
int sendbytes;
struct sockaddr_in servaddr;
char sendline[MAXLINE];
char recvline[MAXLINE];
int OP;


int main(int argc, char const *argv[])
{

  if (argc != 2)
      err_n_die("usage : %s <server address>", argv[0]);

  WSADATA ws;

  if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    printf("WSA failed to initialised .\n");
  else
    printf("WSA initialised successfully !\n");

  // Initialized the Socket
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0)
  {
    printf("The Socket not opened\n");
    WSACleanup();
    exit(EXIT_FAILURE);
  }
  else
    printf("The Socket open Successfully ! %d\n", sockfd);

  ZeroMemory(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVER_PORT);

  // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  OP=inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  //printf("%s", servaddr.sin_addr.s_addr);

  if (OP <= 0)
      err_n_die("inet_pton error for %s", argv[1]);

  nRet = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  if (nRet < 0)
  {
    printf("Connect Failed !\n");
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
  sendbytes = strlen(sendline);

  // send the request

  if (write(sockfd, sendline, sendbytes) != sendbytes)
    err_n_die("write error");

  memset(recvline, 0, MAXLINE);

  // read the server response
  while ((n = read(sockfd, recvline, MAXLINE - 1) > 0))
  {
    printf("%s", recvline);
  }
  if (n < 0)
    err_n_die("read error");

  // _exit(0); //end successfully

  return 0;

  //  return 0;
}

