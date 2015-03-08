#pragma once
#include <string>
using namespace std;
class TestTableClass
{
public:
	TestTableClass(void);
	~TestTableClass(void);
	TestTableClass(const TestTableClass& ttc);
	void SetID(const string& s);
	void SetSour(const string& s);
	void SetDest(const string& s);
	void SetTitle(const string& s);
	void SetContent(const string& s);
	void SetReply(const string& s);
	void SetType(const string& s);
	void SetState(const string& s);
	void SetTime(const string& s);
	void SetType(int i);
	void SetState(int i);
	
	typedef void(TestTableClass::*pfuntion)(const string& s);
	virtual int GetFunctionsPointer(TestTableClass::pfuntion** p);
	void ReleaseFunctionsPointer();
	void GetID(string& s)const;
	void GetSour(string& s)const;
	void GetDest(string& s)const;
	void GetTitle(string& s)const;
	void GetContent(string& s)const;
	void GetReply(string& s)const;
	int GetType()const;
	int GetState()const;
	void GetTime(string& s)const;
private:
	string id,sour,dest,title,content,reply,time;
	int type,state;
	pfuntion* pfs;
};

class wTestTableClass
{
public:
	wTestTableClass(void);
	~wTestTableClass(void);
	wTestTableClass(const wTestTableClass& ttc);
	void SetID(const wstring& s);
	void SetSour(const wstring& s);
	void SetDest(const wstring& s);
	void SetTitle(const wstring& s);
	void SetContent(const wstring& s);
	void SetReply(const wstring& s);
	void SetType(const wstring& s);
	void SetState(const wstring& s);
	void SetTime(const wstring& s);
	void SetType(int i);
	void SetState(int i);
	
	typedef void(wTestTableClass::*pfuntion)(const wstring& s);
	virtual int GetFunctionsPointer(wTestTableClass::pfuntion** p);
	void ReleaseFunctionsPointer();
	void GetID(wstring& s)const;
	void GetSour(wstring& s)const;
	void GetDest(wstring& s)const;
	void GetTitle(wstring& s)const;
	void GetContent(wstring& s)const;
	void GetReply(wstring& s)const;
	int GetType()const;
	int GetState()const;
	void GetTime(wstring& s)const;
private:
	wstring id,sour,dest,title,content,reply,time;
	int type,state;
	pfuntion* pfs;
};

