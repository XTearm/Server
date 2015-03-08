#include <boost\thread.hpp>
#include <boost\bind.hpp>
#include "SocketProcess.h"
#include "Server.h"
#include "Translator.h"
#include "sqlFunction\sqlClass\Tran_MultiByte_WideChar.h"
#include "icClass\Cic.h"
const auto buffsize = 2048;
CSocketProcess::CSocketProcess(int size,CServer* server_)
{
	server = server_;
	sockets = new boost::circular_buffer<boost::asio::ip::tcp::socket*>(size);}


CSocketProcess::~CSocketProcess(void)
{
	while(sockets->size())
	{
		boost::asio::ip::tcp::socket* ps = sockets->front();
		sockets->pop_front();
		server->Push(ps);
	}
	boost::circular_buffer<boost::asio::ip::tcp::socket*>().swap(*sockets);
	delete sockets;
}

bool CSocketProcess::Push(boost::asio::ip::tcp::socket* ps)
{
	bool pushed = false;
	mtx.Lock();
	if(ps&&!sockets->full())
	{
		sockets->push_back(ps);
		pushed = true;
	}
	mtx.UnLock();
	return pushed;
}
void CSocketProcess::Pop()
{
	mtx.Lock();
	boost::asio::ip::tcp::socket* ps = sockets->front();
	sockets->pop_front();
	mtx.UnLock();
	bool is_open = ps->is_open();
	try{
		ps->shutdown(ps->shutdown_both);
	
	ps->close();}
	catch(std::exception&e){
		const char* err = e.what();
	}
	server->Push(ps);
}

UINT CSocketProcess::GetSize()
{
	mtx.Lock();
	int size = sockets->size();
	mtx.UnLock();
	return size;
}
void CSocketProcess::Read(boost::asio::ip::tcp::socket* ps,string &msg)
{
	msg = "";
	bool available = true;
	while(available)
	{
		char buff[buffsize] = {0};
		ps->receive(boost::asio::buffer(buff,buffsize-1));
		msg+=buff;
		available = ps->available()>0;
	}
}
void CSocketProcess::Read(boost::asio::ip::tcp::socket* ps,wstring &msg)
{
	msg = L"";
	bool available = true;
	while(available)
	{
		wchar_t buff[buffsize] = {0};
		ps->receive(boost::asio::buffer(buff,buffsize-2));
		msg+=buff;
		available = ps->available()>0;
	}
}


void CSocketProcess::Write(boost::asio::ip::tcp::socket* ps,string* msg)
{
	
	boost::asio::ip::tcp::socket::send_buffer_size sbs;
	ps->get_option(sbs);
	ps->set_option(boost::asio::ip::tcp::socket::send_buffer_size(msg->size()+5));
	ps->send(boost::asio::buffer(*msg));
	ps->set_option(sbs);
	Pop();
}
void CSocketProcess::Write(boost::asio::ip::tcp::socket* ps,wstring* msg)
{
	int size = msg->size();
	ps->send(boost::asio::buffer((void*)&size,sizeof(size)));
	boost::asio::ip::tcp::socket::send_buffer_size sbs;
	ps->get_option(sbs);
	ps->set_option(boost::asio::ip::tcp::socket::send_buffer_size(msg->size()+5));
	ps->send(boost::asio::buffer(*msg));
	ps->set_option(sbs);
	Pop();
}

void CSocketProcess::process()
{
	while(1)
	{
		if(GetSize()>0)
		{
			boost::asio::socket_base::linger option;
			boost::asio::ip::tcp::socket* ps = sockets->front();
			ps->get_option(option);
			wstring msg = L"";
			wstring res = L"";
			Read(ps,msg);
			Translate(msg,res,ps);
			Write(ps,&res);
		}
		else
		{
			Suspend();
		}
	}

}

bool CSocketProcess::Translate(const string& msg,string& res,boost::asio::ip::tcp::socket* ps)
{
	CTranslator ctl(msg);
	string ver = ctl.GetVerb();
	Functions* fs = server->GetFunctions();
	if(string("GetUnreadCount") == ver)
	{
		int i = fs->getUnreadCount(ctl.GetSubject());
		char buff [10] = {0} ;
		itoa(i,buff,10);
		res = buff;
		return true;
	}
	if(string("GetList")==ver)
	{
		string Su = ctl.GetSubject();
		string n = ctl.GetObj(),s = ctl.GetAdverbial();
		int in = atoi(n.c_str()),is = atoi(s.c_str());
		queue<TestTableClass> qt;
		fs->getList(Su,in,is,qt);
		if(qt.size()==0)
		{
			res = "No such data";
			return false;
		}

		res="#";
		while(qt.size())
		{
			string temp="";
			qt.front().GetID(temp);
			res+="$ID:";res+=temp;qt.front().GetTitle(temp);
			res+="$Title:";res+=temp;qt.front().GetTime(temp);
			res+="$Time:";res+=temp;res+="$";
			qt.pop();
		}
		res+="#";
		return true;
	}
	if(string("GetMessage")==ver)
	{
		string Su = ctl.GetSubject();
		string messageID = ctl.GetObj();
		TestTableClass ttc;
		fs->getMessage(Su,messageID,ttc);
		string temp ;
		res="#";res+="$MessageID:";ttc.GetID(temp);
		if(temp=="")
		{
			res="No such message";
			return false;
		}
		res+=temp;res+="$Title:";ttc.GetTitle(temp);
		res+=temp;res+="$Content:";ttc.GetContent(temp);
		res+=temp;res+="$Time:";ttc.GetTime(temp);
		res+=temp;res+="$#";

		return true;
	}
	if(string("Publish")==ver)
	{
		string Su = ctl.GetSubject();
		string Ob = ctl.GetObj();
		string TitleAndContent = ctl.GetAdverbial();
		smatch what;
		bool b = regex_search(TitleAndContent,what,regex("<Ad><Title/>=(.*?),<Content/>=(.*?)<Ad/>"));
		if(!what.empty())
		{
			if(fs->publish(what[1],what[2],Su,Ob))
			{
				res = "OK";
				return true;
			}
		}
		res = "Fail";
	}
	if(string("Reply")==ver)
	{
		string Su = ctl.GetSubject();
		string Ob = ctl.GetObj();
		string Content = ctl.GetAdverbial();
		if(fs->reply(Ob,Su,Content))
		{
			res = "OK";
			return true;
			
		}
		res = "Fail";
	}
	if(string("Del") == ver)
	{
		string Su = ctl.GetSubject();
		string MessageID = ctl.GetObj();
		if(fs->del(Su,MessageID))
		{
			res="OK";
			return true;
		}

	}
	res="Fail";
	return false;
}

bool CSocketProcess::Translate(const wstring& msg,wstring& res,boost::asio::ip::tcp::socket* ps)
{
	wCTranslator ctl(msg);
	wstring ver = ctl.GetVerb();
	Functions* fs = server->GetFunctions();
	wstring Su = ctl.GetSubject();
	string address = ps->remote_endpoint().address().to_string();
	wstring waddress;
	CTran_MultiByte_WideChar::MToW(address,waddress);
	if(wstring(L"GetIC") == ver )
	{
		
	}
	if(Su==L"")
	{
		res = L"Data Error";
		return false;
	}
	wcout<<Su<<L"\t"<<ver<<endl;
	if(wstring(L"Login")==ver)
	{
		wstring Ad = ctl.GetAdverbial();
		if(fs->login(Su,Ad,waddress))
		{
			res = L"OK";
			return true;
		}
		res = L"Login fail,please check password or userID";
	}
	
	if(!fs->CheckIdAndAddress(Su,waddress))
	{
		res = L"Your ID is offline";
		return false;
	}
	
	if(wstring(L"GetUnreadCount") == ver)
	{
		int i = fs->getUnreadCount(Su);
		wchar_t buff [10] = {0} ;
		_itow(i,buff,10);
		res = buff;
		return true;
	}
	if(wstring(L"GetList")==ver)
	{
		wstring n = ctl.GetObj(),s = ctl.GetAdverbial();
		int in = _wtoi(n.c_str()),is = _wtoi(s.c_str());
		queue<wTestTableClass> qt;
		fs->getList(Su,in,is,qt);
		if(qt.size()==0)
		{
			res = L"No such data";
			return false;
		}
		res = L"<begin>";
		while(qt.size())
		{
			res+=L"<Message>";
			wstring temp=L"";
			qt.front().GetID(temp);
			res+=L"<ID>";res+=temp;qt.front().GetTitle(temp);
			res+=L"<Title>";res+=temp;
			int ty = qt.front().GetType();
			wchar_t cty[5] = {0};
			_itow(ty,cty,10);
			temp = cty;
			res+=L"<Type>";res+=temp;qt.front().GetTime(temp);
			res+=L"<Time>";res+=temp;
			qt.pop();
			res+=L"</></Message>";
		}
		res += L"<end>";
		return true;
	}
	if(wstring(L"GetMessage")==ver)
	{
		wstring messageID = ctl.GetObj();
		wTestTableClass ttc;
		fs->getMessage(Su,messageID,ttc);
		wstring temp ;
		res=L"<Message>";res+=L"MessageID=";ttc.GetID(temp);
		if(temp==L"")
		{
			res=L"No such message";
			return false;
		}
		res+=temp;res+=L"Title=";ttc.GetTitle(temp);
		res+=temp;res+=L"Content=";ttc.GetContent(temp);
		res+=temp;res+=L"Type=";
		wchar_t type[5] = {0};
		_itow(ttc.GetType(),type,10);
		temp = type;
		res+=temp;res+=L"Time=";ttc.GetTime(temp);
		res+=temp;res+=L"<Message>";

		return true;
	}
	if(wstring(L"Publish")==ver)
	{
		wstring TitleAndContent = ctl.GetAdverbial();
		wstring Obs = ctl.GetObj();
		wsmatch what;
		bool b = regex_search(TitleAndContent,what,wregex(L"<Title>(.*?)</Title><Content>([^.]+)</Content>"));
		if(!what.empty())
		{
			wstring ob = Obs;
			wsmatch obwhat;
			while(regex_search(ob,obwhat,wregex(L"(.*?)[;|*]")))
			{
				ob = obwhat[1].str().c_str();
				if(fs->publish(what[1],what[2],Su,ob))
				{
					ob = obwhat[0].second._Ptr;
					regex_search(ob,obwhat,wregex(L"(.*?)[;|*]"));
				}
				else
				{
					res = L"No such friend:";
					res+=ob;
					return false;
				}
				
			}
			res = L"OK";
			return true;
		}
		res = L"Pubilsh fail , check the recipient. ";
	}
	if(wstring(L"Reply")==ver)
	{
		wstring Ob = ctl.GetObj();
		wstring Content = ctl.GetAdverbial();
		switch(fs->reply(Ob,Su,Content))
		{
		case 0:
			res = L"OK";
			return true;
		case 1:
			res = L"No such message!";
			return false;
		case 2:
			res = L"You have no such friend";
			return false;
		}
		res = L"You have no such friend.";
	}
	if(wstring(L"Del") == ver)
	{
		wstring MessageID = ctl.GetObj();
		if(fs->del(Su,MessageID))
		{
			res=L"OK";
			return true;
		}
		res = L"No such messageID:";
		res+= MessageID;
	}
	
	if(wstring(L"Logout") == ver)
	{
		wstring Ad = ctl.GetAdverbial();
		if(fs->logOut(Su,Ad))
		{
			res = L"OK";
			return true;
		}
		res = L"Logout fail,please check the password and userID";
	}
	if(wstring(L"Register")==ver)
	{

	}
	if(wstring(L"AddFriend")==ver)
	{
		wstring Ob = ctl.GetObj();
		if(Su == Ob)
		{
			res = L"Can not add self in the friend list";
			return false;
		}
		switch(fs->AddFriend(Su,Ob))
		{
		case 0:
			res = L"OK";
			return true;
		case 1:
			res = L"No such user.";//res = L"Not such firend or this friend has been in you list";
			return false;
		case 2:
			res = L"This friend has been in you list";
		case 3:
			res = L"Unknow error,please try agein!";
		}
		
	}
	if(wstring(L"GetFriends") == ver)
	{
		queue<CFriendTable> friends;
		if(fs->GetFriends(0,10,Su,friends)&&friends.size()>0)
		{
			res = L"<begin>";
			while(friends.size())
			{
				res+=L"<Friends>";
				wstring temp=L"";
				friends.front().GetUserID(temp);
				res+=L"<UserID>";res+=temp;
				int ty = 0;
				friends.front().GetType(ty);
				res+=L"<type>";
				wchar_t buff[5] = {0};
				_itow(ty,buff,10);
				res+=buff;
				friends.front().GetType(ty);
				memset(buff,0,5);
				_itow(ty,buff,10);
				res+=L"<level>";res+=buff;
				friends.pop();
				res+=L"</></Friends>";
			}
			res += L"<end>";
		}

	}
	
	return false;
}