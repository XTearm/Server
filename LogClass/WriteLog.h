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
	bool open(const string& fileName);//打开、创建日志文档，追加最新日志到ExeSQLError.log，参数：fileName[in]日志文件名
	int Write(const string& str);//写入错误信息，参数：str[in] 提供要写入日志的信息，这些信息将被加在执行时间后面，详细格式型查看ExeSQLError.log
	void close(void);//关闭文件流
};

