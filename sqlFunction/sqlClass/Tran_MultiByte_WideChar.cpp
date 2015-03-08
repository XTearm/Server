#include "Tran_MultiByte_WideChar.h"
#include <Windows.h>

CTran_MultiByte_WideChar::CTran_MultiByte_WideChar(void)
{
}


CTran_MultiByte_WideChar::~CTran_MultiByte_WideChar(void)
{
}

void CTran_MultiByte_WideChar::MToW(const string& str,wstring& wstr,int char_set)
{
	int dwNum = MultiByteToWideChar (char_set, 0, str.c_str(),str.length(), NULL, 0);
	wchar_t* _wstr = new wchar_t[dwNum+1];
	MultiByteToWideChar (char_set, 0, str.c_str(),str.length(), _wstr, dwNum);
	_wstr[dwNum] = L'\0';
	wstr = _wstr;
	delete [] _wstr;
}
void CTran_MultiByte_WideChar::WToM(const wstring& wstr,string& str,int char_set)
{
	int dwNum = WideCharToMultiByte(char_set,NULL,wstr.c_str(),wstr.length(),NULL,0,NULL,FALSE);
	char* _str = new char[dwNum+1];
	WideCharToMultiByte(char_set,NULL,wstr.c_str(),wstr.length(),_str,dwNum,NULL,FALSE);
	_str[dwNum] = '\0';
	str = _str;

	delete []_str;
}