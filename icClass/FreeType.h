#pragma once
#include <iostream>
#include "ft2build.h"
#include FT_FREETYPE_H   
#include FT_GLYPH_H  
using namespace std;
class CFreeType
{
public:
	CFreeType(void);
	~CFreeType(void);
	FT_Face* GetFace(const string& fontName);
	void ReleaseFace(FT_Face* face);
private:
	FT_Library g_ftLib;  
};

