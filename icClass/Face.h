#pragma once
#include <iostream>
#include "ft2build.h"
#include "Face.h"
#include FT_FREETYPE_H   
#include FT_GLYPH_H  
using namespace std;
class CFace
{
public:
	CFace(FT_Face*face,int size);
	~CFace(void);
	void SetSize(int size);
	FT_Bitmap * Render(char uchar);
	FT_Face * GetHandle();
	void Rotate(double angle,double t);
private:
	FT_Face* ftFace;
};

