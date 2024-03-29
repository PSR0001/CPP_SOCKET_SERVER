#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define WINVER WindowsXP

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "8000"
#define SA struct sockaddr

unsigned char buff[4096];
const char *recvbuf2 = "hello";
const char sbuff[4096];

char *htmlRequest , htmlResponse[DEFAULT_BUFLEN];
int recv_size;


//function prototype

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    //Client IP variable
    struct sockaddr_in addr;
    socklen_t addr_len;
    char client_address[DEFAULT_BUFLEN+1];

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

//Previous Code
    // Accept a client socket
    // ClientSocket = accept(ListenSocket, NULL, NULL);
    // if (ClientSocket == INVALID_SOCKET)
    // {
    //     printf("accept failed with error: %d\n", WSAGetLastError());
    //     closesocket(ListenSocket);
    //     WSACleanup();
    //     return 1;
    // }


    // Accept a client socket
    ClientSocket = accept(ListenSocket, (SA*)&addr, &addr_len);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    inet_ntop(AF_INET,&addr,client_address,DEFAULT_BUFLEN);

    printf("Client address : %s\n",client_address);

//     iResult = send(ClientSocket, sbuff, (int)strlen(sbuff), 0);
    
//    if (iResult == SOCKET_ERROR)
//     {
//         printf("send failed with error: %d\n", WSAGetLastError());
//         closesocket(ClientSocket);
//         WSACleanup();
//         return 1;
//     }
    // No longer need server socket
    //closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    int x = 0;
    do
    {
         htmlRequest = "GET / HTTP/1.1\nAccept: text/plain, text/html\n\r\n\r\n\r\n\r\nhello";
    if( send(ClientSocket, htmlRequest , strlen(htmlRequest) , 0) < 0)
        return 1;
    // recv_size = recv(ClientSocket , htmlResponse, sizeof(htmlResponse) , 0);
    // htmlResponse[recv_size] = '\0';
    // printf("%s",htmlResponse);

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {

            printf("Bytes received: %d\n", iResult);
            printf("Bytes received: %s\n", recvbuf);
        }

        // snprintf((char *)buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\r\nHello");
        // printf("LOL : %d\n",x);
       
    } while (1);





    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}


const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
  struct sockaddr_storage ss;
  unsigned long s = size;

  ZeroMemory(&ss, sizeof(ss));
  ss.ss_family = af;

  switch(af) {
    case AF_INET:
      ((struct sockaddr_in *)&ss)->sin_addr = *(struct in_addr *)src;
      break;
    case AF_INET6:
      ((struct sockaddr_in6 *)&ss)->sin6_addr = *(struct in6_addr *)src;
      break;
    default:
      return NULL;
  }
  /* cannot direclty use &size because of strict aliasing rules */
  return (WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, dst, &s) == 0)?
          dst : NULL;
}