#pragma once

#include <Windows.h>
#include <process.h>
class COwnThread
{
public:
	COwnThread(void);
	virtual~COwnThread(void);
	void Lock();
	void Unlock();
	void Join();
	void Suspend();
	void Resume();
protected:
	virtual void process() = 0 ;
	static unsigned __stdcall stdcall(void*);
private:
	HANDLE handle;
};

class COwnMutex
{
public:
	COwnMutex();
	~COwnMutex();
	void Lock();
	void UnLock();
private:
	HANDLE handle;
};
