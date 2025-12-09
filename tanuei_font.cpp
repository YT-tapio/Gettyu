#include"DxLib.h"
#include"tanuei_font.h"
#include<windows.h>

TanueiFont::TanueiFont()
{

}

void TanueiFont::Awake()
{
	const char* file_path = 

	AddFontResourceExA(file_path, FR_PRIVATE, nullptr);
	handle_ = CreateFontToHandle(font_path, size, thick, font_type);
}