#pragma once
#include <iostream>
using namespace std;
class CFriendTable
{
public:
	CFriendTable(void);
	CFriendTable(const CFriendTable&);
	~CFriendTable(void);
	void GetUserID(wstring&u)const;
	void GetName(wstring&n)const;
	void GetType(int&t)const;
	void GetLevel(int&l)const;
	void SetUserID(const wstring&u);
	void SetName(const wstring&n);
	void SetType(const int& t);
	void SetLevel(const int &l);
private:
	wstring userID,name;
	int level,type;
};

