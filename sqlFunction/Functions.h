#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include "sqlClass\CSql.h"
#include "sqlClass\CSqlConnectionPool.h"

#include "TestTableClass.h"
#include "FriendTable.h"
using namespace std;
class Functions
{
public:
	Functions(int min , int max);
	~Functions(void);
	int getUnreadCount(const string& user);
	int getUnreadCount(const wstring& user);
	void getList(const string& user,int num,int size,queue<TestTableClass>& messages);
	void getList(const wstring& user,int num,int size,queue<wTestTableClass>& messages);
	void getMessage(const string & user,const string& MessageID, TestTableClass& message);
	void getMessage(const wstring & user,const wstring& MessageID, wTestTableClass& message);
	bool publish(const string&title,const string& content,const string& sour,const string& dest,const string& reply = "",string* ID = NULL);
	bool publish(const wstring&title,const wstring& content,const wstring& sour,const wstring & dest,const wstring& reply = L"",wstring* ID = NULL);
	bool reply(const string & MessageID,const string& user,const string& content,string* ID = NULL);
	int reply(const wstring & MessageID,const wstring& user,const wstring& content,wstring* ID = NULL);
	bool del(const string& user,const string & messageID);
	bool del(const wstring& user,const wstring & messageID);
	bool login(const string&user,const string& pass,const string& address);
	bool login(const wstring&user,const wstring& pass,const wstring& address);
	bool logOut(const string&user,const string& pass);
	bool logOut(const wstring&user,const wstring& pass);
	bool CheckIdAndAddress(const string&user,const string& address);
	bool CheckIdAndAddress(const wstring&user,const wstring& address);
	int AddFriend(const wstring& user,const wstring& friendID);
	bool GetFriends(int num,int size,const wstring& user,queue<CFriendTable>&friends);
	bool MakeIC(const wstring & ip,const wstring & ic);
private:
	void CreateMessageID(string& id);
	CSqlConnectionPool* pscp;
	mutex mtx;
	int cumulation;
	string master,slave;
};

