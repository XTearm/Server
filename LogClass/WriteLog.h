#pragma once
#include <fstream>
using namespace std;
class CWriteLog
{
public:
	CWriteLog(void);
	~CWriteLog(void);
private:
	ofstream handle;
public:
	bool open(const string& fileName);//�򿪡�������־�ĵ���׷��������־��ExeSQLError.log��������fileName[in]��־�ļ���
	int Write(const string& str);//д�������Ϣ��������str[in] �ṩҪд����־����Ϣ����Щ��Ϣ��������ִ��ʱ����棬��ϸ��ʽ�Ͳ鿴ExeSQLError.log
	void close(void);//�ر��ļ���
};

