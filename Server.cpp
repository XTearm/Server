//#include "ThreadProcess.h"
#include "Server.h"
#include <boost\bind.hpp>
#include <iostream>
#include "SocketProcess.h"
using namespace std;
CServer::CServer(int echoSize_,int ThreadNum_)
{
	thread_rel_handle =  OpenThread(DUPLICATE_SAME_ACCESS,FALSE,GetCurrentThreadId());
	echoSize = echoSize_;
	ThreadNum = ThreadNum_;
	ctps = new COwnThreadPool(ThreadNum);
	sockets = new boost::circular_buffer<boost::asio::ip::tcp::socket*>(echoSize*ThreadNum);
	functions = new Functions(echoSize,echoSize * ThreadNum);
	for(UINT i = 0; i < echoSize * ThreadNum ; i ++ )
	{
		sockets->push_back(new boost::asio::ip::tcp::socket(io));
	}
}


CServer::~CServer(void)
{
	delete ctps;
	while(sockets->size())
	{
		delete sockets->front();
		sockets->pop_front();
	}
	delete sockets;
	handle->close();
	delete handle;
	CloseHandle(thread_rel_handle);
	delete functions;
}

void CServer::Begin(string ip,string port,int listen_wd)
{
	for(UINT i = 0 ; i < ThreadNum ; i ++ )
	{

		CSocketProcess *csp = new CSocketProcess(echoSize,this);
		ctps->Create(csp);
	}
	handle = new boost::asio::ip::tcp::acceptor(io);
	boost::asio::ip::tcp::resolver resolver(io);
	boost::asio::ip::tcp::resolver::query query(ip,port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	handle->open(endpoint.protocol());
	handle->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	handle->bind(endpoint);
	handle->listen(listen_wd);
	//while(1)
	//{
	//	bool isEmpty = IsEmpty();
	//	if(isEmpty)
	//	{
	//		Wait();
	//	}
	//	mtx.Lock();
	//	boost::asio::ip::tcp::socket* ps = sockets->front();
	//	sockets->pop_front();
	//	mtx.UnLock();
	//	bool o = ps->is_open();
	//	handle->accept(*ps);
	//	o = ps->is_open();
	//	boost::circular_buffer<CSocketProcess*>* pts = (boost::circular_buffer<CSocketProcess*>*)ctps->GetHandles();
	//	int index = 0,size = pts[0][0]->GetSize() ;
	//	for(int i = 0 ; i < ctps->GetHandles()->size() ; i ++)
	//	{
	//		CSocketProcess*xxx = pts[0][i];
	//		if(xxx->GetSize()<=size)
	//		{
	//			size = xxx->GetSize();
	//			index = i ;
	//		}
	//	}
	//	pts[0][index]->Push(ps);
	//	pts[0][index]->Resume();
	//}

	boost::asio::ip::tcp::socket* ps = sockets->front();
	handle->async_accept(*ps,boost::bind(&CServer::Accept_Function,this,ps,boost::asio::placeholders::error));
	io.run();
} 
void CServer::Resume()
{
	int x = ResumeThread(thread_rel_handle);
	x = 0 ;
}
void CServer::Wait()
{
	if(SuspendThread(thread_rel_handle)==1)
		ResumeThread(thread_rel_handle);
}

void CServer::Accept_Function(boost::asio::ip::tcp::socket*ps_,const boost::system::error_code& error)
{
	if(error)
		return;
	boost::circular_buffer<CSocketProcess*>* pts = (boost::circular_buffer<CSocketProcess*>*)ctps->GetHandles();
	mtx.Lock();
	sockets->pop_front();
	mtx.UnLock();
	int index = 0,size = pts[0][0]->GetSize() ;
	for(int i = 0 ; i < ctps->GetHandles()->size() ; i ++)
	{
		CSocketProcess*xxx = pts[0][i];
		if(xxx->GetSize()<=size)
		{
			size = xxx->GetSize();
			index = i ;
		}
	}
	pts[0][index]->Push(ps_);
	pts[0][index]->Resume();

	bool isEmpty = IsEmpty();
	if(isEmpty)
	{
		Wait();
	}
	mtx.Lock();
	boost::asio::ip::tcp::socket* ps = ps = sockets->front();
	mtx.UnLock();
	handle->async_accept(*ps,boost::bind(&CServer::Accept_Function,this,ps,boost::asio::placeholders::error));
	
}


void CServer::Push(boost::asio::ip::tcp::socket* ps)
{
	mtx.Lock();
	bool isFull = sockets->full();
	if(ps&&!isFull)
	{
		
		sockets->push_back(ps);
		//if(sockets->size()>0)
			Resume();
	}
	mtx.UnLock();
}

boost::asio::io_service* CServer::GetServerIO()
{
	return &io;
}
bool CServer::IsEmpty()
{
	mtx.Lock();
	bool isEmpty  = sockets->empty();
	mtx.UnLock();
	return isEmpty;
}

Functions* CServer::GetFunctions()
{
	return functions;
}

///////////////
