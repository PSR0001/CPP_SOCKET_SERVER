#include <iostream>
#include <winsock.h>
#include <conio.h>
// macros
#define PORT 9909

using namespace std;

struct sockaddr_in srv;

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

    // Initialized the Socket
    int nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nClientSocket < 0)
    {
        cout << endl<< "The Socket not opened" << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl<< "The Socket open Successfully ! " << nClientSocket << endl<< endl;
    }
    // initialised the environment for sockeaddr structure
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");

    nRet = connect(nClientSocket, (struct sockaddr *)&srv, sizeof(srv));

    if (nRet < 0)
    {
        cout << "Connect Failed !" << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl<< "Connected to Server " << endl;

        char buff[255] = {0,};

        recv(nClientSocket, buff, 255, 0);
        cout<<"Press any key to See the Received message ..."<<endl;
        getchar();
        cout<<endl<<buff<<endl;


        //latest Code
        cout<<"Now send Your message to server . "<<endl;
        while (1)
        {
            fgets(buff,255,stdin);
            send(nClientSocket,buff,256,0);
            cout<<"Press any key to get the response from server ..."<<endl;
            getchar();
            recv(nClientSocket, buff, 255, 0);
            cout<<endl<<buff<<endl<<"Now send next message : "<<endl;
        }
         

    }
    return 0;
}
