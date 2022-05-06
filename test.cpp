/*
BACKLOCK = how many request server can handel per second
fd_set = we can put 64 item . all item act as a kernal obj.



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
		exit(EXIT_FAILURE);
		
	}else{
		cout<< "The Socket open Successfully ! "<<nSocket<<endl;
	}
	
	//initialised the environment for sockeaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	//srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(srv.sin_zero),0,8);
	
	//bind the socket to the local port
	nRet  = bind(nSocket,(sockaddr*)&srv,sizeof(sockaddr));
	if(nRet<0){
		cout<<"Fail to bind the local PORT"<<endl;
		exit(EXIT_FAILURE);
	}
	else
		cout<< "Successfully bind the local PORT"<<endl;
	
	//LIsten the request from client (queues the requests)
	nRet = listen(nSocket,BACKLOCK);
	if(nRet<0){
		cout<<"Fail to start to local port"<<endl;
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
		
		cout<<""<<endl;
	}
	else if(nRet == 0){
		cout<< "Nothing on PORT : "<<PORT<<endl;
		//no connection or any communication request made 
		//none of the socket descriptor are ready
	}
	else{
		//it fail your application should some use full message
		cout<<endl<< "Failed to connect ..."<<endl;
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