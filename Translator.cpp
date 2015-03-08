#include "Translator.h"



CTranslator::CTranslator(const string&str)
{
	//regex("Su=(.*?),Ob=(.*),Ve=(.*),Ad=(.*)");
	
	regex_search(str,what,regex("#Su=(.*?),#Ob=(.*?),#Ve=(.*?),#Ad=(.*?)#"));
}


CTranslator::~CTranslator(void)
{
}

const string CTranslator::GetSubject()
{
	if(what.size()>=2)
		return what[1].str().c_str();
	return "";
}
const string CTranslator::GetObj()
{
	if(what.size()>=3)
		return what[2].str().c_str();
	return "";
}
const string CTranslator::GetVerb()
{
	if(what.size()>=4)
		return what[3].str().c_str();
	return "";
}
const string CTranslator::GetAdverbial()
{
	if(what.size()>=5)
		return what[4].str().c_str();
	return "";
}

///////////////

wCTranslator::wCTranslator(const wstring&str)
{
	/*boost::*/regex_search(str,what,wregex(L"<Su>(.*?)</Su><Ob>(.*?)</Ob><Ve>(.*?)</Ve><Ad>([^.]+)</Ad>"));
}


wCTranslator::~wCTranslator(void)
{
}

const wstring wCTranslator::GetSubject()
{
	if(what.size()>=2)
		return what[1].str().c_str();
	return L"";
}
const wstring wCTranslator::GetObj()
{
	if(what.size()>=3)
		return what[2].str().c_str();
	return L"";
}
const wstring wCTranslator::GetVerb()
{
	if(what.size()>=4)
		return what[3].str().c_str();
	return L"";
}
const wstring wCTranslator::GetAdverbial()
{
	if(what.size()>=5)
		return what[4].str().c_str();
	return L"";
}