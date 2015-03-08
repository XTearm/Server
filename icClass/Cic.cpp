#include "Cic.h"

char * Cic::cs = "0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
Cic::Cic(void)
{
	
	
}

int Cic::Create()
{
	char c[5] = {0};
	double angles[] = {0,0,0,0};
	double t[] = {0,0,0,0};
	RGBQUAD rgbs[4] = {0};
	srand(time(NULL));

	int len = strlen(cs);
	for(int i = 0 ; i < 4; i ++)
	{
		int ir = rand()%len;
		c[i] = cs[ir];
		ir =  (rand()%10)-5;
		double dr = ir/10.0;
		t[i] = dr;
		ir = rand()%90-45;
		angles[i] = ir;
		ir = rand()%5+1;
		rgbs[i].rgbBlue = 255/ir;
		ir = rand()%5+1;
		rgbs[i].rgbGreen = 255/ir;
		ir = rand()%5+1;
		rgbs[i].rgbRed = 255/ir;
	}
	memcpy(chars,c,5);
	CFreeType ft;

	FIBITMAP *bmp = FreeImage_Allocate(200,60,24);
	RGBQUAD bg = {200,200,200,0};
	FreeImage_FillBackground(bmp,&bg);
	for(int i = 0 ; i < 4;i++)
	{
		CFace face(ft.GetFace("comici.ttf"),45);
		face.Rotate(angles[i],t[i]);
		CFaceBitmap bits( face.Render(c[i]));
		bits.ToFreeImage(bmp,i*45+10,0,&rgbs[i]);
		ft.ReleaseFace(face.GetHandle());
	}
	for(int s = 0 ; s <7 ; s++)
	{
		int c = rand()%3;
		switch(c)
		{
		case 0:
			{
				int mr = rand()%10;
				for(int i = 0 ; i < FreeImage_GetWidth(bmp) ; i ++)
				{
				
					FreeImage_SetPixelColor(bmp,i+FreeImage_GetHeight(bmp)/4,tan(i*0.0174+c*mr*5)*FreeImage_GetHeight(bmp)/3,&rgbs[s%4]);
					FreeImage_SetPixelColor(bmp,i+FreeImage_GetHeight(bmp)/4,tan(i*0.0174+c*mr*5)*FreeImage_GetHeight(bmp)/3+1,&rgbs[s%4]);
				}
			}
			break;
			
		case 1:
			{
				int mr = rand()%10;
				for(int i = 0 ; i < FreeImage_GetWidth(bmp) ; i ++)
				{
					FreeImage_SetPixelColor(bmp,i+FreeImage_GetHeight(bmp)/3,-tan(i*0.0174+c*mr*5)*FreeImage_GetHeight(bmp)/3,&rgbs[s%4]);
					FreeImage_SetPixelColor(bmp,i+FreeImage_GetHeight(bmp)/3,-tan(i*0.0174+c*mr*5)*FreeImage_GetHeight(bmp)/3+1,&rgbs[s%4]);
				}
			}
			break;
		case 2:
			{
				int mr = rand()%10;
				for(int i = 0 ; i < FreeImage_GetWidth(bmp) ; i ++)
				{
				
					FreeImage_SetPixelColor(bmp,i+FreeImage_GetHeight(bmp)/2,tan(i*0.0174+c*mr*5)*FreeImage_GetHeight(bmp)/3+1,&rgbs[s%4]);
					FreeImage_SetPixelColor(bmp,i+FreeImage_GetHeight(bmp)/2,tan(i*0.0174+c*mr*5)*FreeImage_GetHeight(bmp)/3+0,&rgbs[s%4]);
				}
			}
			break;
		}
	}
	BYTE bytes[36000] = {0};
	DWORD zl = FreeImage_ZLibGZip(bytes,36000,FreeImage_GetBits(bmp),36000);
	data = new BYTE[zl];
	memcpy(data,bytes,zl);
	crc = FreeImage_ZLibCRC32(32,data,zl);
	FreeImage_Unload(bmp);
	length = zl;
	return zl;
}
int Cic::GetInfo(BYTE* buff,char **c)
{
	memcpy(buff,data,length);
	*c = chars;
	return crc;
}
Cic::~Cic(void)
{
	delete []data;
}
