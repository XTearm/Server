#include "OwnThread.h"


COwnThread::COwnThread(void)
{

	//proc.proccess = &COwnThread::process;
	handle = (HANDLE)_beginthreadex(NULL,0,stdcall,this,0,NULL);
}


COwnThread::~COwnThread(void)
{

	DWORD c = 0;
	GetExitCodeThread(handle,&c);
	if(c==259)
		CloseHandle(handle);
}

void COwnThread::Join()
{
	WaitForSingleObject(handle,INFINITE);
}

void COwnThread::Suspend()
{
	if(1==SuspendThread(handle))
		Resume();
}
void COwnThread::Resume()
{
	ResumeThread(handle);
}

COwnMutex::COwnMutex()
{
	handle = CreateMutex(NULL,FALSE,L"");
}

COwnMutex::~COwnMutex()
{
	CloseHandle(handle);
}

void COwnMutex::Lock()
{
	DWORD dReturn = WaitForSingleObject(handle,INFINITE);
}

void COwnMutex::UnLock()
{
	ReleaseMutex(handle);
}

unsigned COwnThread::stdcall(void*_this)
{
	COwnThread* cthis = (COwnThread*) _this;
	cthis->process();
	return 0;
}
