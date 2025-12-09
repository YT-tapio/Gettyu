#pragma once

class Font
{
private:

	int handle_;

public:

	Font(const char* file_path,const char* font_path, int size, int thick,int font_type);

	~Font();
	
	const int GetHandle() const { return handle_; }
};