#include "CSql.h"


CSql::CSql():handle(0)
{
}


CSql::~CSql(void)
{
}
bool CSql::FromHandle(MYSQL*phandle)
{
	if(phandle==NULL)return false;
	if(mysql_errno(phandle))return false;
	handle = phandle;
	return true;
}
MYSQL* CSql::GetHandle()const
{
	return handle;
}
int CSql::executeSql(string& sql)
{
	if(mysql_query(handle,sql.c_str()))
	return mysql_affected_rows(handle);
	return -1;
}
MYSQL_STMT* CSql::CreateStatment(const string& sql)
{
	MYSQL_STMT* stmt = mysql_stmt_init(handle);
	if( mysql_stmt_prepare(stmt,sql.c_str(),sql.length()))
		return NULL;
	return stmt;
}
void CSql::SetHandle(MYSQL* phandle)
{
	handle = phandle;
}
