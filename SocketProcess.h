#pragma once
#include <iostream>
#include <boost\circular_buffer.hpp>
#include <boost\asio.hpp>
#include "ThreadClass\OwnThread.h"

using namespace std;
class CServer;
class CSocketProcess :
	public COwnThread
{
public:
	CSocketProcess(int size,CServer* server_);
	~CSocketProcess(void);
	bool Push(boost::asio::ip::tcp::socket* ps);
	UINT GetSize();
protected:
	CServer* server;
	COwnMutex mtx;
	boost::circular_buffer<boost::asio::ip::tcp::socket*>*sockets;
	virtual void process();
	void Pop();
	void Read(boost::asio::ip::tcp::socket* ps,string &msg);
	void Read(boost::asio::ip::tcp::socket* ps,wstring &msg);
	void Write(boost::asio::ip::tcp::socket* ps,string* msg);
	void Write(boost::asio::ip::tcp::socket* ps,wstring* msg);
	bool Translate(const string& msg,string& res,boost::asio::ip::tcp::socket* ps);
	bool Translate(const wstring& msg,wstring& res,boost::asio::ip::tcp::socket* ps);
};

