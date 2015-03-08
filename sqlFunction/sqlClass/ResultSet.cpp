#include "ResultSet.h"

ResultSet::ResultSet():handle(0)
{
	
}
bool ResultSet::FromHandle(MYSQL_STMT* phandle)
{
	binds=NULL;
	handle = phandle;
	if(!handle)
		return false;
	if(mysql_stmt_errno(handle))
		return false;
	//////////////////////////
	MYSQL_RES* rs = mysql_stmt_result_metadata(handle);
	if(mysql_stmt_errno(handle))
		return false;
	unsigned int field_count = mysql_num_fields(rs);
	MYSQL_FIELD* fields = mysql_fetch_fields(rs);
	if(mysql_errno(handle->mysql))
		return false;
	binds = new MYSQL_BIND[field_count];
	memset(binds,0,sizeof(MYSQL_BIND)*field_count);
	for(unsigned int i = 0 ; i <field_count ; i ++)
	{
		int len = 0;
		switch(fields[i].type)
		{
		case MYSQL_TYPE_TINY:
		case MYSQL_TYPE_YEAR:
			binds[i].buffer = (char*)new char;
			break;
		case MYSQL_TYPE_SHORT:
			binds[i].buffer = (char*)new char[4];
			break;
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
			binds[i].buffer = (char*)new char[fields[i].length*3];
			break;

		case MYSQL_TYPE_LONG:
		case MYSQL_TYPE_FLOAT:
			binds[i].buffer = (char*)new char[4];
			break;
		case MYSQL_TYPE_TIMESTAMP:
			{
			MY_CHARSET_INFO mci;
			mysql_get_character_set_info(handle->mysql,&mci);
			binds[i].buffer = (char*)new char[fields[i].length*mci.mbmaxlen+1];
			break;}
		case MYSQL_TYPE_LONGLONG:
		case MYSQL_TYPE_DOUBLE:
		case MYSQL_TYPE_DATETIME:
			binds[i].buffer = (char*)new char[8];
			break;

		case MYSQL_TYPE_BIT:
			binds[i].buffer = (char*)new char[(int)((fields[i].length+7)/8)];
			break;
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VAR_STRING:
			binds[i].buffer = (char*)new char[fields[i].length+1];
			break;
		}
		binds[i].buffer_type = fields[i].type;
		binds[i].buffer_length = fields[i].length;
		binds[i].is_null = new my_bool(0);
		binds[i].length = new unsigned long;
	}
	///////////////////////////
	mysql_free_result(rs);
	if(!mysql_stmt_bind_result(handle,binds)&&
	!mysql_stmt_store_result(handle))
		return true;
	return false;
}
ResultSet::~ResultSet(void)
{
	MYSQL_RES* rs =  mysql_stmt_result_metadata(handle);
	unsigned int field_count = mysql_num_fields(rs);
	if(binds)
	{
		for(unsigned int i = 0 ; i < field_count ; i ++ )
		{
			switch(binds[i].buffer_type)
			{
				case MYSQL_TYPE_TINY:
				case MYSQL_TYPE_YEAR:
					delete binds[i].buffer;
					break;
				case MYSQL_TYPE_SHORT:
					delete[] binds[i].buffer;
					break;
				case MYSQL_TYPE_INT24:
				case MYSQL_TYPE_DATE:
				case MYSQL_TYPE_TIME:
					delete[] binds[i].buffer;
					break;

				case MYSQL_TYPE_LONG:
				case MYSQL_TYPE_FLOAT:
					delete[] binds[i].buffer;
					break;
				case MYSQL_TYPE_TIMESTAMP:
					delete[] binds[i].buffer;
					break;
				case MYSQL_TYPE_LONGLONG:
				case MYSQL_TYPE_DOUBLE:
				case MYSQL_TYPE_DATETIME:
					delete[] binds[i].buffer;
					break;

				case MYSQL_TYPE_BIT:
					delete[] binds[i].buffer;
					break;
				case MYSQL_TYPE_STRING:
				case MYSQL_TYPE_VAR_STRING:
					delete[] binds[i].buffer;
					break;
			}
			delete binds[i].is_null;
			delete binds[i].length;
		}
	}
	delete[] binds;
	mysql_free_result(rs);
	mysql_stmt_free_result(handle);
}

int ResultSet::GetCount()
{
	return mysql_stmt_num_rows(handle);
}

int ResultSet::Next()
{
	return mysql_stmt_fetch(handle);
}

int ResultSet::GetInt(int index,bool*canUse)
{
	
	my_bool is_null = 0;
	unsigned long length;
	int value = -1;
	MYSQL_BIND bi[1];
	memset(bi,0,sizeof(bi));
	bi[0].buffer_type = MYSQL_TYPE_LONG;
	bi[0].buffer = (char*)&value;
	bi[0].is_null = & is_null;
	bi[0].length = & length;
	if(mysql_stmt_fetch_column(handle,bi,index,0)&&(canUse))
	{
		*canUse = false;
		return 0;
	}
	if((mysql_stmt_errno(handle)||is_null)&&(canUse))
	{
		*canUse = false;
		return 0;
	}
	
	return value;
}
int ResultSet::GetString(int index,string &value)
{

	my_bool is_null;
	unsigned long length;
	
	MYSQL_BIND bi[1];
	char temp[10] = {0};
	string str="";
	do{
		memset(bi,0,sizeof(bi));
		bi[0].buffer_type = MYSQL_TYPE_STRING;
		bi[0].buffer = temp;
		bi[0].buffer_length=9;
		bi[0].is_null = & is_null;
		bi[0].length = & length;
		int err = mysql_stmt_fetch_column(handle,bi,index,str.length());
		if(err)
			return err;
		if(is_null)
			break;
		str+=temp;
	}while(str.length()<length);
	value=str;
	return 0;
}
int ResultSet::GetString(int index,wstring &value)
{
	my_bool is_null;
	unsigned long length;
	
	MYSQL_BIND bi[1];
	char temp[10] = {0};
	string str="";
	do{
		memset(bi,0,sizeof(bi));
		bi[0].buffer_type = MYSQL_TYPE_VAR_STRING;
		bi[0].buffer = temp;
		bi[0].buffer_length=9;
		bi[0].is_null = & is_null;
		bi[0].length = & length;
		int err = mysql_stmt_fetch_column(handle,bi,index,str.length());
		if(err)
			return err;
		if(is_null)
			return 1;
		str+=temp;
	}while(str.length()<length);
	MY_CHARSET_INFO my_charset_info;
	memset(&my_charset_info,0,sizeof(MY_CHARSET_INFO));
	mysql_get_character_set_info( handle->mysql,&my_charset_info);
	int char_set = CP_ACP;
	if(!strcmp(my_charset_info.csname,"utf8"))char_set = CP_UTF8;

	CTran_MultiByte_WideChar::MToW(str,value,char_set);
	return 0;
}
MYSQL_STMT* ResultSet::GetHandle()
{
	return handle;
}