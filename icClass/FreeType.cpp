#include "FreeType.h"


CFreeType::CFreeType(void)
{
	 FT_Init_FreeType(&g_ftLib);
}


CFreeType::~CFreeType(void)
{
	FT_Done_FreeType(g_ftLib);
}

//FT_New_Face
FT_Face* CFreeType::GetFace(const string& fontName)
{
	FT_Face* face = new FT_Face;
	 FT_New_Face(g_ftLib,fontName.c_str(),0,face);
	 //delete face;
	 return face;
}

void CFreeType::ReleaseFace(FT_Face* face)
{
	FT_Done_Face(*face);
	delete face;
}