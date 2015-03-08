#pragma once
#include <iostream>
#include "ft2build.h"
#include "Face.h"
#include FT_FREETYPE_H   
#include FT_GLYPH_H  
#include "FreeImage.h" 
class CFaceBitmap
{
public:
	CFaceBitmap(FT_Bitmap*bitmap);
	~CFaceBitmap(void);
	int GetWidth();
	int GetHeight();
	void ToFreeImage(FIBITMAP* image,int x,int y,RGBQUAD* fg=0);
private:
	FT_Bitmap *pBmp;
};

