#include<iostream>
#include<winsock.h>
using namespace std;

//macros
#define PORT 8000
#define int_32_ 32
/***********************************************************
 * compile command = "g++ <test>.cpp -o <test2> -lws2_32"
 * compile command = "g++ One.cpp -o One -lws2_32"
 * 
 *  1.gethostname & WSAStartup
 *  2.setsockopt demo
 * 
 *********************************************************/

//structure
struct sockaddr_in srv;

int main(int argc, char const *argv[])
{
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
    
    char strHostName[int_32_]={0,};
    int nRet=gethostname(strHostName,int_32_);
    if(nRet<0){
        cout<<"Call failed !"<<endl;

        // exit();
    }
    else{
        cout<<"The HOST name : "<<strHostName<<endl;
    }
    return 0;
}

