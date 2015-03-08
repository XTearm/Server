#pragma once
#include <boost\circular_buffer.hpp>
#include "OwnThread.h"
class COwnThreadPool
{
public:
	COwnThreadPool(UINT size_);
	~COwnThreadPool(void);
	bool Create(COwnThread* pthread);
	bool Suspend(UINT id);
	bool Resume(UINT id); 
	void SuspendAll();
	void ResumeAll();
	void JoinAll();
	bool Join(UINT id);
	boost::circular_buffer<COwnThread*>*GetHandles();
protected:
	
	boost::circular_buffer<COwnThread*>*handles;
};

