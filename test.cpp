/*
compile command = "g++ test.cpp -o test2 -lws2_32"

*BACKLOCK = how many request server can handel per second
*fd_set = we can put 64 item . all item act as a kernal obj.
#every socket bydefault a blocking socket


*/

#include <iostream> 
#include<winsock.h> 
#include<conio.h>
#define PORT 9909
#define BACKLOCK 5
using namespace std; 


struct sockaddr_in srv;
fd_set fr,fw,fe;
 
//global variable;
int nMaxFD;
 
int main()    
{  
	int nRet=0;
	
	//initialised WSA variables
	WSADATA ws;
	if(WSAStartup(MAKEWORD(2,2),&ws)<0){
		cout<<endl<< "WSA failed to initialised ."<<endl;
	}else{
		cout<< "WSA initialised successfully !"<<endl;
	}
	
	//Initialized the Socket 
	int nSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(nSocket<0){
		cout<<endl<<"The Socket not opened"<<endl;
		WSACleanup();
		exit(EXIT_FAILURE);
		
	}else{
		cout<<endl<< "The Socket open Successfully ! "<<nSocket<<endl<<endl;
	}
	
	//initialised the environment for sockeaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	//srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(srv.sin_zero),0,8);

//setsockopt
	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	
	nRet = setsockopt(nSocket,SOL_SOCKET,SO_REUSEADDR,(const char*)&nOptVal,nOptLen);
	if(!nRet){
		cout<<"The nsockopt call successful."<<endl<<endl;
	}
	else{
		cout<< "The nsockopt call failed."<<endl<<endl;
	}
	
// About the Blocking and non-Blocking socket
	u_long optval=0; //0=blocking & 1=non-blocking
	
	nRet = ioctlsocket(nSocket,FIONBIO, &optval);
	if(nRet !=0){
		cout<< "ioctlsocket call failed."<<endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else{
		cout<< "ioctlsocket call passed."<<endl;
	}
	
	
//bind the socket to the local port
	nRet  = bind(nSocket,(sockaddr*)&srv,sizeof(sockaddr));
	if(nRet<0){
		cout<<"Fail to bind the local PORT"<<endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout<< "Successfully bind the local PORT"<<endl;
	
//LIsten the request from client (queues the requests)
	nRet = listen(nSocket,BACKLOCK);
	if(nRet<0){
		cout<<"Fail to start to local port"<<endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
		cout<< "Started listening to local port"<<endl;
	
	nMaxFD = nSocket;
	
	struct timeval tv;
	tv.tv_sec=1;
	tv.tv_usec=0; 
	
	while(1){
	FD_ZERO(&fe);
	FD_ZERO(&fr);
	FD_ZERO(&fw);
	
	FD_SET(nSocket,&fr);
	FD_SET(nSocket,&fe);
	
	//debuging 
	cout<<endl<<"Before select call : "<<fr.fd_count<<endl;
	
//keep waiting for requests and proceed as per the request
	nRet = select(nMaxFD+1,&fr,&fw,&fe,&tv);
	if(nRet>0){
		//when some one connect or communicate with message over
		//a dedicated connection
		
		cout<<"Data on port ... processing now ..."<<endl;
		//process the request
		if(FD_ISSET(nSocket,&fe)){
			cout<<endl<< "There is an exception . just getaway from here"<<endl;
		}
		
		if(FD_ISSET(nSocket,&fw)){
			cout<<endl<< "Ready to write something."<<endl;
		}
		
		if(FD_ISSET(nSocket,&fr)){
			cout<<endl<< "Ready to read. Somthing new coming..."<<endl;
			//accept the new connection
			
		}
		break;
		
	}
	else if(nRet == 0){
		cout<< "Nothing on PORT : "<<PORT<<endl;
		//no connection or any communication request made 
		//none of the socket descriptor are ready
	}
	else{
		//it fail your application should some use full message
		cout<<endl<< "Failed to connect ..."<<endl;
		
		WSACleanup();
		exit(EXIT_FAILURE);
		
	}
	//debuging 
	cout<<endl<<"After select call : "<<fr.fd_count<<endl;
	//sleep the loop
	Sleep(2000); //for 2s
	}
	
	//end prog
	cout<< "Press any key to Exit";
	getch();
    return 0;  
} 