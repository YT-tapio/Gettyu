#pragma once

class Font
{
private:

	int handle_;

public:

	/// <summary>
	/// フォントの読み込み
	/// </summary>
	/// <param name="file_path">フォントファイルののpath</param>
	/// <param name="font_path">フォントの名前</param>
	/// <param name="size"></param>
	/// <param name="thick"></param>
	/// <param name="font_type"></param>
	Font(const char* file_path,const char* font_path, int size, int thick,int font_type);

	~Font();
	
	const int GetHandle() const { return handle_; }
};