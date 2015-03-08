#pragma once
#include <iostream>
#include <regex>
#include <boost\regex.hpp>
using namespace std;
class CTranslator
{
public:
	CTranslator(const string&str);
	~CTranslator(void);
	const string GetSubject();
	const string GetObj();
	const string GetVerb();
	const string GetAdverbial();
private:
	smatch what;
};

class wCTranslator
{
public:
	wCTranslator(const wstring&str);
	~wCTranslator(void);
	const wstring GetSubject();
	const wstring GetObj();
	const wstring GetVerb();
	const wstring GetAdverbial();
private:
	/*boost::*/wsmatch what;
};
/*
Subject£ºuser;
	Login:Adverbial = pass
	GetUnreadCount:
	GetList:Object = line number,Adverbial = size
	GetMessage:Object = messageID
	Publish:Object = dest,Adverbial = <Ad><Title/>=(.*?),<Content/>=(.*?)<Ad/>
	Reply:Object = messageID,Adverbial = content
	Del:Object = messageID
	Logout: Adverbial = pass
	Normal
	<Su>(.*?)</Su><Ob>(.*?)</Ob><Ve>(.*?)</Ve><Ad>(.*?)</Ad>
	Publish
	<Su>(.*?)</Su><Ob>(.*?)</Ob><Ve>(.*?)</Ve><Ad><Title>(.*?)</Title><Content>(.*?)</Content></Ad>
	<Su>asdf</Su><Ob>asdf</Ob><Ve>asdf</Ve><Ad><Title>asdf</Title><Content>asdf</Content></Ad>

	<Message><Title>asdf</Title><Content>asdf</Content></Message>
	<Message><Title>asdf</Title><Content>asdf</Content></Message>
	<Message><Title>asdf</Title><Content>asdf</Content></Message>
	<Message><Title>asdf</Title><Content>asdf</Content></Message>
*/
