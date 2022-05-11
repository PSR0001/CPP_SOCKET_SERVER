
#define WIN32_LEAN_AND_MEAN
#define WINVER WindowsXP

#include <iostream>
#include <winsock.h>
#include <conio.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>  

#include <windows.h>
//#include <winsock2.h>
#include <ws2tcpip.h>
// macros
#define PORT 80

using namespace std;

struct sockaddr_in srv;

int inet_pton(int af, const char *src, void *dst);
// Global variable
int nClientSocket;

int main(int argc, char const *argv[])
{
    int nRet;

    // initialised WSA variables
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        cout << endl
             << "WSA failed to initialised ." << endl;
    }
    else
    {
        cout << "WSA initialised successfully !" << endl;
    }

    if (argc != 2)
        cout << endl
             << "usage : %s <server address>" << argv[0] << endl;

    // Initialized the Socket
    int nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nClientSocket < 0)
    {
        cout << endl
             << "The Socket not opened" << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl
             << "The Socket open Successfully ! " << nClientSocket << endl
             << endl;
    }
    // initialised the environment for sockeaddr structure

    ZeroMemory(&srv, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    // srv.sin_addr.s_addr = inet_addr("172.217.197.102");

    if (inet_pton(AF_INET, argv[1], &srv.sin_addr) <= 0)
        cout << "inet_pton error for " << argv[1] << endl;

    nRet = connect(nClientSocket, (struct sockaddr *)&srv, sizeof(srv));

    if (nRet < 0)
    {
        cout << "Connect Failed !" << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl
             << "Connected to Server " << endl;

        char buff[4096] = {
            0,
        };

        recv(nClientSocket, buff, 255, 0);
        cout << "Press any key to See the Received message ..." << endl;
        getchar();
        cout << endl
             << buff << endl;

        // latest Code
        cout << "Now send Your message to server . " << endl;
        while (1)
        {
            cout << "Enter the message : ";
            fgets(buff, 255, stdin);

            send(nClientSocket, buff, 256, 0);

            recv(nClientSocket, buff, 256, 0);
            cout << endl
                 << buff << endl
                 << "Now send next message : " << endl
                 << endl;
        }
    }
    return 0;
}

int inet_pton(int af, const char *src, void *dst)
{
    struct sockaddr_storage ss;
    int size = sizeof(ss);
    char src_copy[INET6_ADDRSTRLEN + 1];

    ZeroMemory(&ss, sizeof(ss));
    /* stupid non-const API */
    strncpy(src_copy, src, INET6_ADDRSTRLEN + 1);
    src_copy[INET6_ADDRSTRLEN] = 0;

    if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0)
    {
        switch (af)
        {
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