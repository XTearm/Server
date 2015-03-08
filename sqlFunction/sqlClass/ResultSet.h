#pragma once
#include <windows.h>
#include <mysql.h>
#include<string>
#include "Tran_MultiByte_WideChar.h"
using namespace std;
class ResultSet
{
public:
	ResultSet();
	~ResultSet(void);
	int GetCount();
	int Next();
	bool FromHandle(MYSQL_STMT*phandle);
	int GetInt(int index,bool*canUse);
	int GetString(int index,string &value);
	int GetString(int index,wstring &value);
	MYSQL_STMT* GetHandle();
private:
	MYSQL_STMT* handle;
	MYSQL_BIND* binds;
};

