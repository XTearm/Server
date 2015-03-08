#include "OwnThreadPool.h"
#include <iostream>
using namespace std;
COwnThreadPool::COwnThreadPool(UINT size)
{
	handles = new boost::circular_buffer<COwnThread*>(size);
}


COwnThreadPool::~COwnThreadPool(void)
{
	while(handles->size())
	{
		COwnThread * pt = handles->front();
		handles->pop_front();
		delete pt;
	}
	boost::circular_buffer<COwnThread*>().swap(*handles);
	delete handles;
}


bool COwnThreadPool::Create(COwnThread* pthread)
{
	if(handles->full())
		return false;
	handles->push_back(pthread);
	return true;
}


bool COwnThreadPool::Suspend(UINT id)
{
	if(id<handles->size())
	{
		(*handles)[id]->Suspend();
		return true;
	}
	return false;
}

bool COwnThreadPool::Resume(UINT id)
{
	if(id<handles->size())
	{
		(*handles)[id]->Resume();
		return true;
	}
	return false;
}

void COwnThreadPool::SuspendAll()
{
	for(UINT i = 0 ; i < handles->size() ; i ++ )
		(*handles)[i]->Suspend();
}

void COwnThreadPool::ResumeAll()
{
	for(UINT i = 0 ; i < handles->size() ; i ++ )
		(*handles)[i]->Resume();
}

void COwnThreadPool::JoinAll()
{
	for(UINT i = 0 ; i < handles->size() ; i ++ )
	{
		(*handles)[i]->Join();
	}
}

bool COwnThreadPool::Join(UINT id)
{
	if(id>handles->size())
		return false;
	(*handles)[id]->Join();
	return true;
}

boost::circular_buffer<COwnThread*>*COwnThreadPool::GetHandles()
{
	return handles;
}
