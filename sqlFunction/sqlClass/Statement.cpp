#include "Statement.h"
#include "Tran_MultiByte_WideChar.h"

Statement::Statement():handle(0)
{
	
}


Statement::~Statement(void)
{
	if(!handle)
		return;
	mysql_stmt_close(handle);
	ClearCache();
}
bool Statement::FromHandle(MYSQL_STMT*phandle)
{
	if(phandle==NULL)
		return false;
	if(mysql_stmt_errno(phandle))
		return false;
	handle = phandle;
	return true;
}
void Statement::SetInt(int value)
{
	MYSQL_BIND mb;
	memset(&mb,0,sizeof(mb));
	mb.buffer_type = FIELD_TYPE_LONG;
	mb.buffer = new int(value);
	vmb.push_back(mb);
}
void Statement::SetString(const string & value)
{
	MYSQL_BIND mb;
	memset(&mb,0,sizeof(mb));
	mb.buffer_type = MYSQL_TYPE_STRING;
	mb.buffer = new char[value.length()+1];
	strcpy((char*)mb.buffer,value.c_str());
	mb.buffer_length = value.length();
	vmb.push_back(mb);
}

void Statement::SetString(const wstring & value)
{
	MYSQL_RES* rs = mysql_stmt_result_metadata(handle);
	MYSQL_BIND mb;
	memset(&mb,0,sizeof(mb));
	mb.buffer_type = MYSQL_TYPE_STRING;
	
	string str = "";

	MY_CHARSET_INFO mci;
	mysql_get_character_set_info(handle->mysql,&mci);
	int char_set = 0;
	if(!strcmp(mci.csname,"utf8"))char_set = CP_UTF8;
	CTran_MultiByte_WideChar::WToM(value,str,char_set);
	mb.buffer = new char[str.length()+1];
	strcpy((char*)mb.buffer,str.c_str());
	mb.buffer_length = str.length();
	vmb.push_back(mb);
}

int Statement::ExecuteUpdate()
{
	MYSQL_BIND* pmb = new MYSQL_BIND[vmb.size()];
	memset(pmb,0,vmb.size()*sizeof(MYSQL_BIND));
	for(unsigned int i = 0 ; i < vmb.size() ; i ++)
	{
		pmb[i] = vmb[i];
	}

	if(mysql_stmt_bind_param(handle,pmb))
	{
		delete [] pmb;
		return -1;
	}
	if(mysql_stmt_execute(handle))
	{
		delete [] pmb;
		return -1;
	}
	
	delete [] pmb;
	return mysql_stmt_affected_rows(handle);
}
MYSQL_STMT* Statement::ExecuteQuery()
{
	MYSQL_BIND* pmb = new MYSQL_BIND[vmb.size()];
	memset(pmb,0,vmb.size()*sizeof(MYSQL_BIND));
	for(unsigned int i = 0 ; i < vmb.size() ; i ++)
	{
		pmb[i] = vmb[i];
	}
	if(mysql_stmt_bind_param(handle, pmb))
	{
		delete []pmb;
		return NULL;
	}

	if(mysql_stmt_execute(handle))
	{
		delete [] pmb;
		return NULL;
	}
	delete [] pmb;
	return handle;
}

MYSQL_STMT* Statement::GetHandle()
{
	return handle;
}

void Statement::ClearCache()
{
	for(unsigned int i = 0 ; i < vmb.size() ; i ++)
	{
		switch(vmb[i].buffer_type)
		{
		case MYSQL_TYPE_LONG:
			{
				int* p = (int*)vmb[i].buffer;
				delete p;
			}
			break;
		case MYSQL_TYPE_STRING:
			{
				char* p = (char*)vmb[i].buffer;
				delete[] p;
			}
			break;
		}
	}
	while(vmb.size()>0)
	{
		vmb.pop_back();
	}
	vector<MYSQL_BIND>().swap(vmb);
}