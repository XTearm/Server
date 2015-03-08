#include "TestTableClass.h"

TestTableClass::TestTableClass(void):pfs(0)
{
	
	SetID("");
	
	
	SetSour("");

	
	SetDest("");

	
	SetTitle("");

	
	SetContent("");

	
	SetReply("");

	SetType(0);

	SetState(0);
}


TestTableClass::~TestTableClass(void)
{
}
TestTableClass::TestTableClass (const TestTableClass& ttc)
{
	string stemp ;
	ttc.GetID(stemp);
	SetID(stemp);
	
	ttc.GetSour(stemp);
	SetSour(stemp);

	ttc.GetDest(stemp);
	SetDest(stemp);

	ttc.GetTitle(stemp);
	SetTitle(stemp);

	ttc.GetContent(stemp);
	SetContent(stemp);

	ttc.GetReply(stemp);
	SetReply(stemp);

	ttc.GetTime(stemp);
	SetTime(stemp);

	SetType(ttc.GetType());

	SetState(ttc.GetState());
}
void TestTableClass::SetID(const string& s){
	id = s;
}
void TestTableClass::SetSour(const string& s){
	sour = s;
}
void TestTableClass::SetDest(const string& s){
	dest = s;
}
void TestTableClass::SetTitle(const string& s){
	title = s;
}
void TestTableClass::SetContent(const string& s){
	content = s;
}
void TestTableClass::SetReply(const string& s){
	reply = atoi(s.c_str());
}
void TestTableClass::SetType(const string& s){
	type = atoi(s.c_str());
}
void TestTableClass::SetState(const string& s){
	state = atoi(s.c_str());
}
void TestTableClass::SetTime(const string& s){
	time = s;
}

void TestTableClass::SetType(int i){
	type = i;
}
void TestTableClass::SetState(int i){
	state = i;
}
int TestTableClass::GetFunctionsPointer(TestTableClass::pfuntion** p)
{
	pfs = new pfuntion[9];
	pfs[0] = &TestTableClass::SetID;
	pfs[1] = &TestTableClass::SetType;
	pfs[2] = &TestTableClass::SetState;
	pfs[3] = &TestTableClass::SetSour;
	pfs[4] = &TestTableClass::SetDest;
	pfs[5] = &TestTableClass::SetTitle;
	pfs[6] = &TestTableClass::SetContent;
	pfs[7] = &TestTableClass::SetReply;
	pfs[8] = &TestTableClass::SetTime;
	*p = pfs;
	return 9;
}
void TestTableClass::ReleaseFunctionsPointer()
{
	delete pfs;
	pfs = 0;
}

void TestTableClass::GetID(string& s)const{
	s = id;
}
void TestTableClass::GetSour(string& s)const{
	s = sour;
}
void TestTableClass::GetDest(string& s)const{
	s = dest;
}
void TestTableClass::GetTitle(string& s)const{
	s = title;
}
void TestTableClass::GetContent(string&s)const{
	s = content;
}
void TestTableClass::GetReply(string& s)const{
	s = reply;
}
int TestTableClass::GetType()const{
	return type;
}
int TestTableClass::GetState()const{
	return state;
}

void TestTableClass::GetTime(string& s)const{
	s = time;
}

//wide


wTestTableClass::wTestTableClass(void):pfs(0)
{
	
	SetID(L"");
	
	
	SetSour(L"");

	
	SetDest(L"");

	
	SetTitle(L"");

	
	SetContent(L"");

	
	SetReply(L"");

	SetType(0);

	SetState(0);
}


wTestTableClass::~wTestTableClass(void)
{
}
wTestTableClass::wTestTableClass (const wTestTableClass& ttc)
{
	wstring stemp ;
	ttc.GetID(stemp);
	SetID(stemp);
	
	ttc.GetSour(stemp);
	SetSour(stemp);

	ttc.GetDest(stemp);
	SetDest(stemp);

	ttc.GetTitle(stemp);
	SetTitle(stemp);

	ttc.GetContent(stemp);
	SetContent(stemp);

	ttc.GetReply(stemp);
	SetReply(stemp);

	ttc.GetTime(stemp);
	SetTime(stemp);

	SetType(ttc.GetType());

	SetState(ttc.GetState());
}
void wTestTableClass::SetID(const wstring& s){
	id = s;
}
void wTestTableClass::SetSour(const wstring& s){
	sour = s;
}
void wTestTableClass::SetDest(const wstring& s){
	dest = s;
}
void wTestTableClass::SetTitle(const wstring& s){
	title = s;
}
void wTestTableClass::SetContent(const wstring& s){
	content = s;
}
void wTestTableClass::SetReply(const wstring& s){
	reply = _wtoi(s.c_str());
}
void wTestTableClass::SetType(const wstring& s){
	type = _wtoi(s.c_str());
}
void wTestTableClass::SetState(const wstring& s){
	state = _wtoi(s.c_str());
}
void wTestTableClass::SetTime(const wstring& s){
	time = s;
}

void wTestTableClass::SetType(int i){
	type = i;
}
void wTestTableClass::SetState(int i){
	state = i;
}
int wTestTableClass::GetFunctionsPointer(wTestTableClass::pfuntion** p)
{
	pfs = new pfuntion[9];
	pfs[0] = &wTestTableClass::SetID;
	pfs[1] = &wTestTableClass::SetType;
	pfs[2] = &wTestTableClass::SetState;
	pfs[3] = &wTestTableClass::SetSour;
	pfs[4] = &wTestTableClass::SetDest;
	pfs[5] = &wTestTableClass::SetTitle;
	pfs[6] = &wTestTableClass::SetContent;
	pfs[7] = &wTestTableClass::SetReply;
	pfs[8] = &wTestTableClass::SetTime;
	*p = pfs;
	return 9;
}
void wTestTableClass::ReleaseFunctionsPointer()
{
	delete pfs;
	pfs = 0;
}

void wTestTableClass::GetID(wstring& s)const{
	s = id;
}
void wTestTableClass::GetSour(wstring& s)const{
	s = sour;
}
void wTestTableClass::GetDest(wstring& s)const{
	s = dest;
}
void wTestTableClass::GetTitle(wstring& s)const{
	s = title;
}
void wTestTableClass::GetContent(wstring&s)const{
	s = content;
}
void wTestTableClass::GetReply(wstring& s)const{
	s = reply;
}
int wTestTableClass::GetType()const{
	return type;
}
int wTestTableClass::GetState()const{
	return state;
}

void wTestTableClass::GetTime(wstring& s)const{
	s = time;
}
