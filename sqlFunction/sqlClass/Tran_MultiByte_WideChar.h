#pragma once
#include <iostream>
using namespace std;
class CTran_MultiByte_WideChar
{
public:
	CTran_MultiByte_WideChar(void);
	~CTran_MultiByte_WideChar(void);
	static void MToW(const string& str,wstring& wstr,int char_set = 65001);
	static void WToM(const wstring& wstr,string& str,int char_set = 65001);
};

