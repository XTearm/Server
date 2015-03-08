#pragma once
#include <iostream>
#include <windows.h>
#include <mysql.h>
using namespace std;
class CSql
{
public:
	CSql();
	~CSql(void);
	bool FromHandle(MYSQL*phandle);
	MYSQL* GetHandle()const ;
	int executeSql(string& sql);
	MYSQL_STMT*CreateStatment(const string& sql);
private:
	MYSQL* handle;
public:
	void SetHandle(MYSQL* phandle);
};
