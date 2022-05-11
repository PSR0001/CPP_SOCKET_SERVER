#include <winsock.h>
#include <conio.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>  
// macros
#define PORT 9909


struct sockaddr_in srv;

// Global variable
int nClientSocket;

int main(int argc, char const *argv[])
{
    int nRet;

    // initialised WSA variables
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
        printf("WSA failed to initialised .\n");
    else
        printf("WSA initialised successfully !\n");
    

    // Initialized the Socket
    int nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nClientSocket < 0)
    {
        printf("The Socket not opened\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
         printf( "The Socket open Successfully ! %d\n", nClientSocket );
    
    // initialised the environment for sockeaddr structure
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");

    nRet = connect(nClientSocket, (struct sockaddr *)&srv, sizeof(srv));

    if (nRet < 0)
    {
        printf("Connect Failed !\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Connected to Server \n");
        char buff[4096] = {
            0,
        };

        recv(nClientSocket, buff, 255, 0);
        printf( "Press any key to See the Received message ...\n");
        getchar();
        printf( "Buffer : \n",buff );

        // latest Code
        printf("Now send Your message to server . ");
        while (1)
        {
            printf("Enter the message : \n");
            fgets(buff, 255, stdin);
            //debugging
            //cout<<endl<<buff<<endl<<endl;
            send(nClientSocket, buff, 256, 0);
            //cout << "Press any key to get the response from server ..." << endl;
            //getchar();
            recv(nClientSocket, buff, 256, 0);
            printf("\n%s\n" ,buff );
                printf("Now send next message : \n" );
        }
    }
    return 0;
}
