// ServerClass.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <boost\thread.hpp>
#include <iostream>
#include "Server.h"
#include "Translator.h"
using namespace std;
int eachSize= 10,threadNum = 5,listenSize = 5;
CServer* server;
unsigned __stdcall fun(LPVOID){
	server = new CServer(eachSize,threadNum);
	server->Begin("127.0.0.1","3070",listenSize);
	//server = new CServer(30,3);
	//server->Begin("127.0.0.1","3070",100);
	cout<<endl;
	return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF); 

	cout<<"eachSize:"<<endl;cin>>eachSize;
	cout<<"threadNum:"<<endl;cin>>threadNum;
	cout<<"listenSize:"<<endl;cin>>listenSize;
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,fun,NULL,0,NULL);
	
	cout<<"the server running...\nPlease input Stop_server to stop the server;"<<endl;
	string comand = "";
	while(cin>>comand)
	{
		if(comand == "Stop_server")
			break;
	}
	cout<<"Stopping server..."<<endl;
	server->GetServerIO()->stop();
	cout<<"the server stopped"<<endl;
	Sleep(3000);
	delete server;
	return 0;
}

