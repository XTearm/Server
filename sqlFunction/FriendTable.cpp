#include "FriendTable.h"


CFriendTable::CFriendTable(void)
{
}
CFriendTable::CFriendTable(const CFriendTable& cft)
{
	wstring buff;
	cft.GetUserID(buff);
	SetUserID(buff);

	cft.GetName(buff);
	SetName(buff);

	int t = 0 ;
	cft.GetType(t);
	SetType(t);
}

CFriendTable::~CFriendTable(void)
{
}
void CFriendTable::GetUserID(wstring&u)const
{
	u = userID;
}
void CFriendTable::GetName(wstring&n)const
{
	n = name;
}
void CFriendTable::GetType(int&t)const
{
	t = type;
}
void CFriendTable::GetLevel(int&l)const
{
	l = level;
}
void CFriendTable::SetUserID(const wstring&u)
{
	userID = u;
}
void CFriendTable::SetName(const wstring&n)
{
	name = n;
}
void CFriendTable::SetType(const int& t)
{
	type = t;
}
void CFriendTable::SetLevel(const int&l)
{
	level = l;
}