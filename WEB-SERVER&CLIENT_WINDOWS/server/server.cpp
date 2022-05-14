/*
compile command = "g++ server.cpp -o server -lws2_32"

*/

#include <iostream>
#include <winsock2.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <strings.h>
#include <windows.h>
#include <sstream>

#include <ws2tcpip.h>
#include <winsock.h>
#include <conio.h>
#include <stdint.h>

using namespace std;

#define PORT 3000
#define BACKLOCK 5
#define MAX_BUFFER 5120


string text;
stringstream stream;
struct sockaddr_in srv;

// global variable;
int nSocket, fd_client;

int main()
{
    int nRet = 0;
    FILE *sendFile = fopen("index.html", "r");
    if (sendFile == NULL) /* check it the file was opened */
    {
        printf("File not open");
        exit(EXIT_FAILURE);
    }

    // initialised WSA variables
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
        cout << endl
             << "WSA failed to initialised ." << endl;
    else
        cout << "WSA initialised successfully !" << endl;

    // Initialized the Socket
    nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        cout << endl
             << "The Socket not opened" << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
        cout << endl
             << "The Socket open Successfully ! " << nSocket << endl
             << endl;

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
    {
        cout << "The nsockopt call successful." << endl
             << endl;
    }
    else
        cout << "The nsockopt call failed." << endl
             << endl;

    // About the Blocking and non-Blocking socket
    u_long optval = 0; // 0=blocking & 1=non-blocking

    nRet = ioctlsocket(nSocket, FIONBIO, &optval);
    if (nRet != 0)
    {
        cout << "ioctlsocket call failed." << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
        cout << "ioctlsocket call passed." << endl;

    // bind the socket to the local port
    nRet = bind(nSocket, (sockaddr *)&srv, sizeof(sockaddr));
    if (nRet < 0)
    {
        cout << "Fail to bind the local PORT" << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
        cout << "Successfully bind the local PORT " << PORT << endl;

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
    //-----------------------Open html file------------------------------

    while (1)
    {
        int nLen = sizeof(struct sockaddr);

        int fd_client = accept(nSocket, NULL, &nLen);
        if (fd_client == INVALID_SOCKET)
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            closesocket(nSocket);
            exit(EXIT_FAILURE);
        }

        else
        {

            fseek(sendFile, 0L, SEEK_END);
            /* you can use a stringstream, it's cleaner */
            stream << "HTTP/1.1 200 OK\nContent-length: " << ftell(sendFile) << "\n";
            fseek(sendFile, 0L, SEEK_SET);

            text = stream.str();

            send(fd_client, text.c_str(), text.length(), 0);

            cout << "Sent : " << text << endl;

            text = "Content-Type: text/html\n\n";
            send(fd_client, text.c_str(), text.length(), 0);

            cout << "Sent : %s" << text << endl;
            while (feof(sendFile) == 0)
            {
                int numread;
                char sendBuffer[MAX_BUFFER];

                numread = fread(sendBuffer, sizeof(unsigned char), 3000, sendFile);
                if (numread > 0)
                {
                    char *sendBuffer_ptr;

                    sendBuffer_ptr = sendBuffer;
                    do
                    {
                        fd_set wfd;
                        timeval tm;

                        FD_ZERO(&wfd);
                        FD_SET(fd_client, &wfd);

                        tm.tv_sec = 10;
                        tm.tv_usec = 0;
                        /* first call select, and if the descriptor is writeable, call send */
                        if (select(1 + fd_client, NULL, &wfd, NULL, &tm) > 0)
                        {
                            int numsent;

                            numsent = send(fd_client, sendBuffer_ptr, numread, 0);
                            if (numsent == -1)
                                exit(EXIT_FAILURE);
                            sendBuffer_ptr += numsent;
                            numread -= numsent;
                        }
                    } while (numread > 0);
                }
            }
        }
        /* don't forget to close the file. */

        closesocket(fd_client);
        
        // Sleep(500);
        //--------------------------------------------------------------
    }

    fclose(sendFile);
    // end prog
    cout << "Press any key to Exit";
    getch();
    return 0;
}