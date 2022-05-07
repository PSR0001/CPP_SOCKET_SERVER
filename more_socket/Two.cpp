#include <iostream>
#include <winsock.h>
#include <stdio.h>
using namespace std;

// macros
#define PORT 8000
#define int_32_ 32
/***********************************************************
 * compile command = "g++ <test>.cpp -o <test2> -lws2_32"
 * compile command = "g++ Two.cpp -o Two -lws2_32"
 *
 *  1.gethostbyname & WSAStartup
 *
 *  Find system IP address :)
 *
 *********************************************************/

// structure
struct sockaddr_in srv;
struct hostent *remoteHost;
struct in_addr addr;

int main()
{
    WSADATA ws;
    int iResults;

    char **pAlias;

    iResults = WSAStartup(MAKEWORD(2, 2), &ws);

    if (iResults != 0)
    {
        return 1;
    }

    char strHostName[32] = {
        0,
    };
    int nRet = gethostname(strHostName, int_32_);
    if (nRet != 0)
    {
        cout << endl
             << "Not able to find the HOSTNAME. " << endl;
        return 1;
    }

    remoteHost = gethostbyname(strHostName);
    if (strHostName[0] != '\0')
    {
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++)
        {
            cout << endl
                 << *pAlias;
        }

        printf("\tAddress type : ");
        switch (remoteHost->h_addrtype)
        {
        case AF_INET:
            cout << endl
                 << "AF_INET";
            srv.sin_family = AF_INET;
            break;

        case AF_NETBIOS:
            cout << endl
                 << "AF_NETBIOS";
            break;

        default:
            break;
        }
        cout << endl
             << "Addr len : " << remoteHost->h_length;
        int i = 0;
        if (remoteHost->h_addrtype == AF_INET)
        {
            while (remoteHost->h_addr_list[i] != 0)
            {
                addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
                cout << endl
                     << inet_ntoa(addr);
            }
        }
        else if (remoteHost->h_addrtype == AF_NETBIOS)
        {
            cout << endl
                 << "NETBIOS address was returned.0";
        }

        srv.sin_port = htons(PORT);
        srv.sin_addr.s_addr = addr.s_addr;
        memset(&(srv.sin_zero), 0, 8);
    }

    return 0;
}
