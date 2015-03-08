#include "Functions.h"
#include "sqlClass\Statement.h"
#include "sqlClass\ResultSet.h"
#include "..\LogClass\WriteLog.h"
#include <iostream>
#include <fstream>
#include <regex>
using namespace std;
Functions::Functions(int min,int max)
{


	string info[6];
	string condition[] = {"host=(.*?)","user=(.*?)","pswd=(.*?)","masterdatabase=(.*?)","slavedatabase=(.*?)","port=(.*?)"};
	ifstream ini; 
	ini.open( "DatabaseConfig.ini");
	if(!ini.is_open())
		return ;
	for(int i = 0 ; i < 6 ; i ++)
	{
		getline(ini,info[i]);
		smatch result;
		if(!regex_search(info[i],result,regex(condition[i])))
			return ;
		info[i] = result.suffix().str();
	}
	int port = atoi(info[5].c_str());
	master = info[3];
	slave = info[4];
	pscp = new CSqlConnectionPool(info[0],info[1],info[2],port);
	cumulation = 0 ;
	if(!pscp->Init(min,max))
	{
		CWriteLog c;
		c.open("LogFile.Log");
		string error = "Database init fail:";
		error+=pscp->GetLastError();
		c.Write(error);
		cout<<"Server can not start,the LogFile has been recorded some infomation"<<endl;
		exit(-1);
		//throw "DB pool init fial";
	}
	
	
}


Functions::~Functions(void)
{
	delete pscp;
}

int Functions::getUnreadCount(const string& user)
{

	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("select * from viewfrommessages where dest = ? and state = 0 and type = 1")))
		{
			stmt.SetString(user);
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				pscp->ReleaseConnection(&s);
				int count = rs.GetCount();
				return count;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	
	return -1;
}

int Functions::getUnreadCount(const wstring& user)
{

	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("select * from viewfrommessages where dest = ? and state = 0 and type = 1")))
		{
			stmt.SetString(user);
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				pscp->ReleaseConnection(&s);
				int count = rs.GetCount();
				return count;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	
	return -1;
}

void Functions::getList(const string& user,int num,int size,queue<TestTableClass>& messages)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("SELECT messageID,title,time FROM viewfrommessages where ((dest = ? and type = 1) or (sour = ? and type = 0)) order by time desc limit ?,?")))
		{
			stmt.SetString(user);
			stmt.SetString(user);
			stmt.SetInt(num);
			stmt.SetInt(size);
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				pscp->ReleaseConnection(&s);
				while(!rs.Next())
				{
					const char* error = mysql_stmt_error(rs.GetHandle());
					TestTableClass ttc;
					string str;
					rs.GetString(0,str);
					ttc.SetID(str);

					rs.GetString(1,str);
					ttc.SetTitle(str);

					rs.GetString(2,str);
					ttc.SetTime(str);

					messages.push(ttc);
				}
				
				return;
			}
		}
		pscp->ReleaseConnection(&s);
	}
}

void Functions::getList(const wstring & user,int num,int size,queue<wTestTableClass>& messages)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("SELECT messageID,title,time,type FROM viewfrommessages where ((dest = ? and type = 1) or (sour = ? and type = 0)) order by time desc limit ?,?")))
		{
			stmt.SetString(user);
			stmt.SetString(user);
			stmt.SetInt(num);
			stmt.SetInt(size);
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				pscp->ReleaseConnection(&s);
				while(!rs.Next())
				{
					const char* error = mysql_stmt_error(rs.GetHandle());
					wTestTableClass ttc;
					wstring str;
					rs.GetString(0,str);
					ttc.SetID(str);

					rs.GetString(1,str);
					ttc.SetTitle(str);

					rs.GetString(2,str);
					ttc.SetTime(str);
					bool cu = false;
					int type = rs.GetInt(3,&cu);
					ttc.SetType(type);
					messages.push(ttc);
				}
				
				return;
			}
		}
		pscp->ReleaseConnection(&s);
	}
}


void Functions::getMessage(const string & user,const string& MessageID,TestTableClass& message)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("SELECT * FROM viewfrommessages where messageID = ? and dest = ? and type = 1")))
		{
			stmt.SetString(MessageID);
			stmt.SetString(user);
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				pscp->ReleaseConnection(&s);
				TestTableClass::pfuntion *pfs;
				int pfss = message.GetFunctionsPointer(&pfs);
				rs.Next();
				for(int i = 0 ; i < pfss ; i ++)
				{
					string str = "";
					rs.GetString(i,str);
					(message.*pfs[i])(str);
				}
				message.ReleaseFunctionsPointer();
				return;
			}
		}
		pscp->ReleaseConnection(&s);
	}
}


void Functions::getMessage(const wstring & user,const wstring& MessageID,wTestTableClass& message)
{
	MYSQL *my_conn = pscp->GetConnection();
	MYSQL *my_conn_master = pscp->GetConnection();
	CSql s,s_master;
	Statement stmt,ostmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,slave)&&my_conn_master&&pscp->SelectDB(my_conn_master,master))
	{
		if(s.FromHandle(my_conn)&&s_master.FromHandle(my_conn_master)
			&& stmt.FromHandle(s.CreateStatment("SELECT * FROM viewfrommessages where messageID = ? and ((dest = ? and type = 1) or (sour = ? and type = 0))"))
			&&ostmt.FromHandle(s_master.CreateStatment("UPDATE messages set state = 1 where messageID = ? and type = 1;")))
		{
			stmt.SetString(MessageID);
			stmt.SetString(user);
			stmt.SetString(user);
			ostmt.SetString(MessageID);
			if(rs.FromHandle(stmt.ExecuteQuery())&&ostmt.ExecuteUpdate()>-1)
			{

				pscp->ReleaseConnection(&s);
				pscp->ReleaseConnection(&s_master);
				wTestTableClass::pfuntion *pfs;
				int pfss = message.GetFunctionsPointer(&pfs);
				rs.Next();
				for(int i = 0 ; i < pfss ; i ++)
				{
					wstring str = L"";
					rs.GetString(i,str);
					(message.*pfs[i])(str);
				}
				message.ReleaseFunctionsPointer();
				return;
			}
		}
		pscp->ReleaseConnection(&s);
	}
}


bool Functions::publish(const string&title,const string& content,const string& sour,const string & dest,const string& reply,string* ID)
{

	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt,ostmt;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)
			&& stmt.FromHandle(s.CreateStatment("INSERT INTO messages(messageID, type, state, sour, dest, title, content,replyto,time) VALUES (?, 1, 0, ?, ?, ?, ?, ? ,null)"))
			&&ostmt.FromHandle(s.CreateStatment("INSERT INTO messages(messageID, type, state, sour, dest, title, content,replyto,time) VALUES (?, 0, 0, ?, ?, ?, ?, ? ,null)")))
		{
			string id;
			CreateMessageID(id);
			stmt.SetString(id);
			stmt.SetString(sour);
			stmt.SetString(dest);
			stmt.SetString(title);
			stmt.SetString(content);
			stmt.SetString(reply);

			CreateMessageID(id);
			ostmt.SetString(id);
			ostmt.SetString(sour);
			ostmt.SetString(dest);
			ostmt.SetString(title);
			ostmt.SetString(content);
			ostmt.SetString(reply);
			//int xxx = ostmt.ExecuteUpdate();
			if(ostmt.ExecuteUpdate()>-1&&stmt.ExecuteUpdate()>-1)
			{
				if(ID!=NULL)
					*ID = id;
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	return false;
}
bool Functions::publish(const wstring&title,const wstring& content,const wstring& sour,const wstring & dest,const wstring& reply,wstring* ID)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt,ostmt,ustmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)
			&& stmt.FromHandle(s.CreateStatment("INSERT INTO messages(messageID, type, state, sour, dest, title, content,replyto,time) VALUES (?, 1, 0, ?, ?, ?, ?, ? ,null)"))
			&&ostmt.FromHandle(s.CreateStatment("INSERT INTO messages(messageID, type, state, sour, dest, title, content,replyto,time) VALUES (?, 0, 0, ?, ?, ?, ?, ? ,null)"))
			&&ustmt.FromHandle(s.CreateStatment("SELECT type from friends where user = ? and friendID = ?")))
		{
			string id;wstring wid;
			CreateMessageID(id);
			CTran_MultiByte_WideChar::MToW(id,wid);
			stmt.SetString(wid);
			stmt.SetString(sour);
			stmt.SetString(dest);
			stmt.SetString(title);
			stmt.SetString(content);
			stmt.SetString(reply);

			CreateMessageID(id);
			CTran_MultiByte_WideChar::MToW(id,wid);
			ostmt.SetString(wid);
			ostmt.SetString(sour);
			ostmt.SetString(dest);
			ostmt.SetString(title);
			ostmt.SetString(content);
			ostmt.SetString(reply);
			ustmt.SetString(sour);
			ustmt.SetString(dest);
			if(rs.FromHandle(ustmt.ExecuteQuery())&&rs.GetCount()>0&&stmt.ExecuteUpdate()>-1&&ostmt.ExecuteUpdate()>-1)
			{
				if(ID!=NULL)
					*ID = wid;
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	return false;
}
bool Functions::reply(const string & MessageID,const string& user,const string& content,string* ID)
{
	bool su = false;
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	TestTableClass ttc;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		Statement stmt;
		ResultSet rs;
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("SELECT sour,dest,title from viewfrommessages where messageID = ? and user = ? and type = 1")))
		{
			stmt.SetString(MessageID);
			stmt.SetString(user);
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				int x = rs.GetCount();
				pscp->ReleaseConnection(&s);
				if(!rs.Next())
				{
					
					string str = "";
					rs.GetString(0,str);
					ttc.SetSour(str);
				
					str = "";
					rs.GetString(1,str);
					ttc.SetDest(str);

					str = "";
					rs.GetString(2,str);
					string restr = "reply:";restr+=str;
					ttc.SetTitle(restr);
					su = true;
				}
			}
		}
		pscp->ReleaseConnection(&s);
	}
	string sour,dest,title;
	ttc.GetSour(sour);
	ttc.GetDest(dest);
	ttc.GetTitle(title);
	if(su&&publish(title,content,dest,sour,MessageID,ID))
	{
		
		su = true;
	}
	else
		su = false;
	return su;
}
int Functions::reply(const wstring & MessageID,const wstring& user,const wstring& content,wstring* ID)
{
	int su = false;
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	wTestTableClass ttc;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		Statement stmt;
		ResultSet rs;
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("SELECT sour,dest,title from viewfrommessages where messageID = ? and dest = ? and type = 1")))
		{
			stmt.SetString(MessageID);
			stmt.SetString(user);
			if(rs.FromHandle(stmt.ExecuteQuery())&&rs.GetCount()>0)
			{
				pscp->ReleaseConnection(&s);
				if(!rs.Next())
				{
					
					wstring str = L"";
					rs.GetString(0,str);
					ttc.SetSour(str);
				
					str = L"";
					rs.GetString(1,str);
					ttc.SetDest(str);

					str = L"";
					rs.GetString(2,str);
					wstring restr = L"reply:";restr+=str;
					ttc.SetTitle(restr);
					su = true;
				}
			}
			else
			{
				//pscp->ReleaseConnection(&s);
				su = 1;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	wstring sour,dest,title;
	ttc.GetSour(sour);
	ttc.GetDest(dest);
	ttc.GetTitle(title);
	if(su==0&&publish(title,content,dest,sour,MessageID,ID))
	{
		
		su = 0;
	}
	else
		su = 2;
	return su;
}
bool Functions::del(const string& user,const string & messageID)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("delete from messages where messageID = ? and ((dest = ? and type = 1) or (sour = ? and type = 0))")))
		{
			stmt.SetString(messageID);
			stmt.SetString(user);
			stmt.SetString(user);
			if(stmt.ExecuteUpdate()>0)
			{
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	return false;
}
bool Functions::del(const wstring& user,const wstring & messageID)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("delete from messages where messageID = ? and ((dest = ? and type = 1) or (sour = ? and type = 0))")))
		{
			stmt.SetString(messageID);
			stmt.SetString(user);
			stmt.SetString(user);
			if(stmt.ExecuteUpdate()>0)
			{
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	return false;
}
int last_second = 0 ;
void Functions::CreateMessageID(string& id)
{
	
	char buff[5] = {0};
	
	id = "";
	struct tm newtime;
	time_t long_time;
	time(&long_time);
	localtime_s(&newtime,&long_time);
	int year = 1900+newtime.tm_year;
	int mday = newtime.tm_mday;
	int hms[3] = {newtime.tm_hour,newtime.tm_min,newtime.tm_sec};
	_itoa(year,buff,10);
	id+=buff;

	_itoa(mday,buff,10);
	int len = strnlen(buff,5);
	for(len;len<3;len++)
		id+="0";
	id+=buff;

	for(int i = 0 ; i < 3; i ++ )
	{
		_itoa(hms[i],buff,10);
		if(strnlen(buff,5)==1)id+="0";
		id+=buff;
	}
	/////////////////
	mtx.lock();
	if(hms[2]!=last_second)
		cumulation =0;
	int cum_temp = cumulation++;
	cumulation%=10000;
	last_second = hms[2];
	mtx.unlock();
	/////////////////
	_itoa(cum_temp,buff,10);
	len = strnlen(buff,5);
	for(len;len<4;len++)
		id+="0";
	id+=buff;
}

bool Functions::login(const string&user,const string& pass,const string& address)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	if(my_conn&&pscp->SelectDB(my_conn,"viewfromeuser"))
	{
		if(s.FromHandle(my_conn)
			&&stmt.FromHandle(s.CreateStatment("update user set ip = ? where userID = ? and password = ?")))
		{
			stmt.SetString(address);
			stmt.SetString(user);
			stmt.SetString(pass);
			if(stmt.ExecuteUpdate()>0)
			{
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
	}
	pscp->ReleaseConnection(&s);
	return false;
}

bool Functions::login(const wstring&user,const wstring& pass,const wstring& address)
{
	int res = 0 ;
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt,ostmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)
			&& stmt.FromHandle(s.CreateStatment("select type from user where userID = ? and password = ?"))
			&&ostmt.FromHandle(s.CreateStatment("update user set ip = ? where userID = ?")))
		{
			stmt.SetString(user);
			stmt.SetString(pass);
			ostmt.SetString(address);
			ostmt.SetString(user);
			
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				if(rs.GetCount()>0){
					ostmt.ExecuteUpdate();
					pscp->ReleaseConnection(&s);
					return true;
				}
			}
		}
	}
	pscp->ReleaseConnection(&s);
	return false;
}

bool Functions::logOut(const string&user,const string& pass)
{
	return login(user,pass,"");
}

bool Functions::logOut(const wstring&user,const wstring& pass)
{
	return login(user,pass,L"");
}

bool Functions::CheckIdAndAddress(const string&user,const string& address)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,slave))
	{
		if(s.FromHandle(my_conn)
			&&stmt.FromHandle(s.CreateStatment("select type from user where userID = ? and ip = ?")))
		{
			stmt.SetString(user);
			stmt.SetString(address);
			rs.FromHandle(stmt.ExecuteQuery());
			if(rs.GetCount()>0)
			{
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
	}
	pscp->ReleaseConnection(&s);
	return false;
}

bool Functions::CheckIdAndAddress(const wstring&user,const wstring& address)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)
			&&stmt.FromHandle(s.CreateStatment("select type from user where userID = ? and ip = ?")))
		{
			stmt.SetString(user);
			stmt.SetString(address);
			if(rs.FromHandle(stmt.ExecuteQuery())&&(rs.GetCount()>0))
			{
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
	}
	pscp->ReleaseConnection(&s);
	return false;
}


int Functions::AddFriend(const wstring&user , const wstring&friendID)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt,ostmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)
			&& stmt.FromHandle(s.CreateStatment("select type,level from user where userID = ?"))
			&&ostmt.FromHandle(s.CreateStatment("insert into friends(user,friendID,level,type) values(?,?,?,?)")))
		{
			stmt.SetString(friendID);
			ostmt.SetString(user);
			ostmt.SetString(friendID);
			if(rs.FromHandle(stmt.ExecuteQuery())&&rs.GetCount()>0)
			{
				rs.Next();
				bool canUse = false;
				int ty = rs.GetInt(0,&canUse);
				int le = rs.GetInt(1,&canUse);
				ostmt.SetInt(le);
				ostmt.SetInt(ty);
				
				if(ostmt.ExecuteUpdate()>0)
				{
					
					pscp->ReleaseConnection(&s);
					return 0;
				}
				else
				{
					pscp->ReleaseConnection(&s);
					return 1;
				}

			}
			else
			{
				pscp->ReleaseConnection(&s);
				return 2;
			}
		}
	}
	pscp->ReleaseConnection(&s);
	return 3;
}

bool Functions::GetFriends(int num,int size,const wstring& user,queue<CFriendTable>&friends)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("SELECT friendID,level,type FROM friends where user = ?")))
		{
			stmt.SetString(user);
			if(rs.FromHandle(stmt.ExecuteQuery()))
			{
				pscp->ReleaseConnection(&s);
				while(!rs.Next())
				{
					CFriendTable cft;
					wstring buff;
					rs.GetString(0,buff);
					
					int t = rs.GetInt(1,NULL);
					cft.SetUserID(buff);
					cft.SetType(t);

					t =rs.GetInt(2,NULL);
					cft.SetLevel(t);
					friends.push(cft);
				}
				
				return true;
			}
		}
		pscp->ReleaseConnection(&s);
	}
	return false;
}

bool Functions::MakeIC(const wstring & ip,const wstring& ic)
{
	MYSQL *my_conn = pscp->GetConnection();
	CSql s;
	Statement stmt;
	//ResultSet rs;
	if(my_conn&&pscp->SelectDB(my_conn,master))
	{
		if(s.FromHandle(my_conn)&&stmt.FromHandle(s.CreateStatment("INSERT INTO ic(ip,ic) values(?,?)")))
		{
			stmt.SetString(ip);
			stmt.SetString(ic);
			if(stmt.ExecuteUpdate()>0)
			{
				pscp->ReleaseConnection(&s);
				return true;
			}
		}
	}
	pscp->ReleaseConnection(&s);
}