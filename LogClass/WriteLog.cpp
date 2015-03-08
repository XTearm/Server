#include "WriteLog.h"
#include <time.h>

CWriteLog::CWriteLog(void)
{
	
}


CWriteLog::~CWriteLog(void)
{
	if(handle.is_open())
		handle.close();
}


bool CWriteLog::open(const string& fileName)
{
	handle.open(fileName.c_str(),ios::app);
	return handle.is_open();
}


int CWriteLog::Write(const string& str)
{
	time_t t = time(NULL);
	struct tm* current_time = localtime(&t);//获取本地时间
	handle<<1900+current_time->tm_year<<"-"<<current_time->tm_mon+1<<"-"<<current_time->tm_mday<<"-"
		<<current_time->tm_hour<<":"<<current_time->tm_min<<":"<<current_time->tm_sec<<"\t"
		<<str.c_str()<<endl;
	return 0;
}


void CWriteLog::close(void)
{
	handle.close();
}
