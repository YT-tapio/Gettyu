#include"DxLib.h"
#include"font.h"
#include<windows.h>

Font::Font(const char* file_path, const char* font_path,int size,int thick,int font_type)
{
	AddFontResourceExA(file_path,FR_PRIVATE,nullptr);
	handle_ = CreateFontToHandle(font_path, size, thick, font_type);
}

Font::~Font()
{
	DeleteFontToHandle(handle_);
}