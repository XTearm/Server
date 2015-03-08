#include "CSqlConnectionPool.h"

bool CSqlConnectionPool::isExist = false;
string CSqlConnectionPool::lastError = "";
CSqlConnectionPool::CSqlConnectionPool(string & h,string & u,string& ps,int p)
{
	host = h;
	user = u ;
	pswd = ps;
	port = p;
	currentSize=0;

}


CSqlConnectionPool::~CSqlConnectionPool(void)
{
	while(vm.size()>0)
	{
		DeleteConnection(vm.front());
		vm.pop();
	}
	queue<MYSQL*>().swap(vm);
	mysql_thread_end();
}
bool CSqlConnectionPool::Init(unsigned int minCon,unsigned int maxCon)
{
	if(!isExist)
		isExist = true;
	else
		return false;
	max = maxCon;
	unsigned int errorCount=0;
	for(unsigned int i = 0 ; i < minCon ; i ++)
	{
		MYSQL* pmysql;
		pmysql = AddConnection();
		if(pmysql)
		{
			vm.push(pmysql);
		}
		else
		{
			i--;
			errorCount++;
			if(errorCount>10)
				return false;
		}
	}
	return true;
}
bool CSqlConnectionPool::SelectDB(MYSQL* handle,const string& database,const string& character_set)
{
	if(!handle)
		return false;
	
	mysql_set_character_set(handle,character_set.c_str());
	if(mysql_errno(handle))
	{
		lastError = mysql_error(handle);
		return false;
	}
	mysql_select_db(handle,database.c_str());
	if(mysql_errno(handle))
	{
		lastError = mysql_error(handle);
		false;
	}
	return true;
}
MYSQL* CSqlConnectionPool::GetConnection()
{
	
	MYSQL* pmysql = NULL;
	mtx.lock();
	if(vm.size()>0)
	{
		
		 pmysql = vm.front();
		 vm.pop();
		 
	}
	else if(currentSize <max)
	{
		pmysql = AddConnection();
	}
	mtx.unlock();
	return pmysql;
}
void CSqlConnectionPool::DeleteConnection(MYSQL* pmysql)
{
	mysql_close(pmysql);
}
void CSqlConnectionPool::ReleaseConnection(CSql* pmysql)
{
	if(pmysql==NULL||pmysql->GetHandle()==NULL)
		return;
	mtx.lock();
	vm.push(pmysql->GetHandle());
	mtx.unlock();
	pmysql->SetHandle(NULL);
}
MYSQL* CSqlConnectionPool::AddConnection()
{
	MYSQL* pmysql = mysql_init((MYSQL*)NULL);
	if(pmysql)
	{
		if(mysql_real_connect(pmysql,host.c_str(),user.c_str(),pswd.c_str(),
				NULL,port,NULL,0))
		{
			currentSize++;
			return pmysql;
		}
	}
	lastError = mysql_error(pmysql);
	return NULL;
}

const string& CSqlConnectionPool::GetLastError()
{
	return lastError;
}