/***********************************************
 *  errno - system global variable
 *  htons - host to network short
 *  
 * compile command = "g++ Server.cpp -o server -lws2_32"
 * 
 *  steps:
 *      1.Initiate the Socket environment
 *      2.open socket listener
 * 
 * 
 * ********************************************/

#include <iostream>
#include<stdio.h>
#include <winsock.h>
#include <conio.h>
using namespace std;

#define PORT 8000
#define BACKLOCK 5
//Global Variables
int nRet=0; //return socket
int nSocket; //Listener socket

struct sockaddr_in srv;


int main()
{
    //Initiate the Socket environment
    WSADATA ws;
    nRet=WSAStartup(MAKEWORD(2,2),&ws);
    if (nRet < 0){
        printf("\nCannot Initialised !\n");
        WSACleanup();
		exit(EXIT_FAILURE);
    }
    else
        printf("Your Socket is Ready\n");
    
    //open socket listener
    nSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (nSocket <0)
    {
         printf("\nCannot Initialised Listener SOCKET  : %d\n",errno);
        WSACleanup();
		exit(EXIT_FAILURE);
    }
        else
        printf("Your listener Socket is Ready\n");

	// initialised the environment for sockeaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	// srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(srv.sin_zero), 0, 8);

    // bind the socket
    nRet = bind(nSocket, (sockaddr *)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		cout << "Fail to bind the local PORT" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout << "Successfully bind the local PORT" << endl;


	// LIsten the request from client (queues the requests)
	nRet = listen(nSocket, BACKLOCK);
	if (nRet < 0)
	{
		cout << "Fail to start to local port" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout << "Started listening to local port" << endl;

    return 0;
}

