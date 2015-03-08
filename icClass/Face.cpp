#include "Face.h"


CFace::CFace(FT_Face*face,int size)
{
	ftFace=face;
	FT_Set_Char_Size(*ftFace,size<<6,size<<6,96,96);
}


CFace::~CFace(void)
{
}

void CFace::SetSize(int size)
{
	FT_Set_Char_Size(*ftFace,size<<6,size<<6,96,96); 
}
FT_Bitmap * CFace::Render(char uchar)
{
	FT_Load_Glyph(*ftFace,FT_Get_Char_Index(*ftFace,uchar),FT_LOAD_DEFAULT);
	FT_Render_Glyph((*ftFace)->glyph,FT_RENDER_MODE_NORMAL); 
	return  &(*ftFace)->glyph->bitmap;
}

FT_Face* CFace::GetHandle()
{
	return ftFace;
}

void CFace::Rotate(double angle,double t)
{
	FT_Matrix x = {0x10000L,t*0x10000L,0,0x10000L};
	FT_Matrix matrix = {(FT_Fixed)(cos( 3.14/180*angle)*0x10000L),(FT_Fixed)(sin(3.14/180*angle)*0x10000L),
						(FT_Fixed)(-sin(3.14/180*angle)*0x10000L),(FT_Fixed)(cos(3.14/180*angle)*0x10000L)};
	FT_Matrix_Multiply(&matrix,&x);
	FT_Set_Transform(*ftFace,&x,0);
}
