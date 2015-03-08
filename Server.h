#pragma once
#include "SocketProcess.h"
#include "ThreadClass\OwnThreadPool.h"
#include "sqlFunction\Functions.h"
class CSocketProcess;
class CServer
{
public:
	CServer(int echoSize,int ThreadNum);
	~CServer(void);
	void Push(boost::asio::ip::tcp::socket* ps);
	void Begin(string ip,string port,int listen_wd);
	void Accept_Function(boost::asio::ip::tcp::socket*ps,const boost::system::error_code& error);
	boost::asio::io_service* GetServerIO();
	void Wait();
	void Resume();
	bool IsEmpty();
	Functions* GetFunctions();
protected:
	COwnThreadPool *ctps;
	boost::circular_buffer<boost::asio::ip::tcp::socket*>*sockets;
	UINT echoSize,ThreadNum;
	boost::asio::io_service io;
	COwnMutex mtx;
	boost::asio::ip::tcp::acceptor* handle;
	HANDLE thread_rel_handle;
	Functions* functions;
};
