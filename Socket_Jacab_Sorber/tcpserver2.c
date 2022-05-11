

#define WIN32_LEAN_AND_MEAN
#define WINVER WindowsXP
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


#include <winsock.h>
#include <conio.h>

#define PORT 5000
#define BACKLOCK 5
#define SA struct sockaddr
#define MAXLINE 4096


struct sockaddr_in srv;

// global variable;
int nMaxFD;
int nArrClient[10];
int nSocket;
int connfd,listenfd;
  char *buff2[MAXLINE + 1] = {
        0,
    };

int main()
{
	int nRet = 0;

	// initialised WSA variables
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
		printf( "WSA failed to initialised .\n");
	else
		printf( "WSA initialised successfully !\n");
	

	// Initialized the Socket
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		printf("The Socket not opened\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("The Socket open Successfully ! %d\n" ,nSocket );
	

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
		printf("The nsockopt call successful.\n" );
	
	else
		printf("The nsockopt call failed.\n" );
	

	// About the Blocking and non-Blocking socket
	u_long optval = 0; // 0=blocking & 1=non-blocking

	nRet = ioctlsocket(nSocket, FIONBIO, &optval);
	if (nRet != 0)
	{
		printf( "ioctlsocket call failed. \n" );
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("ioctlsocket call passed.\n" );
	

	// bind the socket to the local port
	nRet = bind(nSocket, (SA *)&srv, sizeof(srv));
	if (nRet < 0)
	{
		printf("Fail to bind the local PORT\n" );
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("Successfully bind the local PORT %d\n",PORT);

	// LIsten the request from client (queues the requests)
	nRet = listen(nSocket, BACKLOCK);
	if (nRet < 0)
	{
		printf("Fail to start to local port\n" );
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		printf("Started listening to local port\n" );

	d
	// end prog
	printf("Press any key to Exit\n" );
	getch();
	return 0;
}