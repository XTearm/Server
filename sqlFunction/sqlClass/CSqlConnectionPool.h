#pragma once
#include <windows.h>
#include <mysql.h>
#include <iostream>
#include <queue>
#include <mutex>
#include "CSql.h"
using namespace std;

class CSqlConnectionPool
{
public:
	CSqlConnectionPool(string & host,string & user,string& pswd,int port);
	~CSqlConnectionPool(void);
	bool Init(unsigned int minCon,unsigned int maxCon);
	bool SelectDB(MYSQL* handle,const string &database,const string& character_set = "utf8");
	MYSQL* GetConnection();//when the value return NULL the connection is unusefull
	const string& GetLastError();
	void ReleaseConnection(CSql* pmysql);
private :
	MYSQL* AddConnection();
	void DeleteConnection(MYSQL*pmysql);
	queue <MYSQL*> vm;
	int max,currentSize,port;
	string host,user,pswd;
	mutex mtx;//œﬂ≥Ãª•≥‚¡ø
	static bool isExist;
	static string lastError;
};

