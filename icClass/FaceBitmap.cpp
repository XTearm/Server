#include "FaceBitmap.h"


CFaceBitmap::CFaceBitmap(FT_Bitmap*bitmap)
{
	pBmp = bitmap;
	
}


CFaceBitmap::~CFaceBitmap(void)
{
}

int CFaceBitmap::GetWidth()
{
	return pBmp->width;
}
int CFaceBitmap::GetHeight()
{
	return pBmp->rows;
}

void CFaceBitmap::ToFreeImage(FIBITMAP* image,int x,int y,RGBQUAD* fg)
{
	bool newfg=false;
	static int l = 2;
	RGBQUAD bg ;
	if(!fg)
	{
		fg = new RGBQUAD();
		memset(fg,0,sizeof(RGBQUAD));
		newfg = true;
	}
	int w = GetWidth();
	int h = GetHeight();
	for ( int i=0;i<h;i+=l )  
	{  
		l+=2;l%=3;
		for ( int j=0;j<w;j+=2 )  
		{  
			if((pBmp->buffer[i*w+j]!=0||pBmp->buffer[i*w+j]!=0|| pBmp->buffer[i*w+j]!=0))
			{
				FreeImage_SetPixelColor(image,j+x,h-i+y,fg);
				
			}
			else
			{
				
				FreeImage_GetPixelColor(image,j+x,h-i+y,&bg);
				FreeImage_SetPixelColor(image,j+x,h-i+y,&bg);
			}
		}  
		
	}
	if(newfg)
		delete fg;
}