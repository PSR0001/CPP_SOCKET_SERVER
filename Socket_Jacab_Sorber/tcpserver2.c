

#define WIN32_LEAN_AND_MEAN
#define WINVER WindowsXP

#include <winsock2.h>
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
#include <windows.h>

#include <ws2tcpip.h>
#include <winsock.h>
#include <conio.h>
#include <stdint.h>

#define PORT 5000
#define BACKLOCK 5
#define SA struct sockaddr
#define MAXLINE 4096


struct sockaddr_in srv,server_addr,client_addr;

int fd_server,fd_client;


// global variable;
int nMaxFD;
int nArrClient[10];
int nSocket;
int connfd, listenfd;
char buff2[MAXLINE + 1] = {
	0,
};

char webpage[] =
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n\r\n"
	"<!DOCTYPE html>\r\n"
	"<html lang='en'>\r\n"
	"<head><title>Document</title></head>\r\n"
	"<body>\r\n"
	"<center><h1>I am served by C server.</h1></center>\r\n"
	"</body>\r\n"
	"</html>\r\n";

int main()
{
	
	int nRet = 0;
	
    struct addrinfo *result = NULL;

	// initialised WSA variables
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
		printf("WSA failed to initialised .\n");
	else
		printf("WSA initialised successfully !\n");

	// Initialized the Socket
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		printf("The Socket not opened\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("The Socket open Successfully ! %d\n", nSocket);

	// initialised the environment for sockeaddr structure

	ZeroMemory(&srv, sizeof(srv));
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	// srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(srv.sin_zero), 0, 8);

	// setsockopt
	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&nOptVal, nOptLen);
	if (!nRet)
		printf("The nsockopt call successful.\n");

	else
		printf("The nsockopt call failed.\n");


	// bind the server

	nRet = bind (nSocket,result->ai_addr, (int)result->ai_addrlen);
	if (nRet<0)
	{
		printf("Bind Error\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	  printf("Bind Successfully!");

	// LIsten the request from client (queues the requests)
	nRet = listen(nSocket, BACKLOCK);
	if (nRet < 0)
	{
		printf("Fail to start to local port\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("Started listening to local port\n");


	while(1){

		fd_client=accept(nSocket,(SA*)&client_addr,(int*)sizeof(client_addr));

		if(fd_client==INVALID_SOCKET){
			printf("ERROR %s\n",WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		else{
			printf("Client Connected ... ");

			memset(buff2,0,MAXLINE);
		
		read(fd_client,buff2,MAXLINE);

		printf("\n%s\n",buff2);

		write(fd_client,webpage,sizeof(webpage)-1);
		
		
		}

		



	}

	// end prog
	printf("Press any key to Exit\n");
	getch();
	return 0;
}