#pragma once

#include <iostream>   
#include "FreeType.h"
#include "FaceBitmap.h"
#include <time.h>
#include <stdio.h>
class Cic
{
public:
	Cic(void);
	~Cic(void);
	int Create();
	int GetInfo(BYTE* buff,char**c);
private:
	static char* cs;
	BYTE* data;
	int crc,length;
	char chars[5];
};

