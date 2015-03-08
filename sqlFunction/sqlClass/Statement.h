#pragma once
#include <vector>
#include <windows.h>
#include <mysql.h>
using namespace std;
class Statement
{
public:
	Statement();
	~Statement(void);
	bool FromHandle(MYSQL_STMT* phandle);
	void SetInt(int value);
	void SetString(const string& value);
	void SetString(const wstring & value);
	int ExecuteUpdate();
	MYSQL_STMT* ExecuteQuery();
	MYSQL_STMT* GetHandle();
	void ClearCache();
private:

	MYSQL_STMT* handle;
	vector<MYSQL_BIND> vmb;
};

