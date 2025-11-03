#pragma once
#include"DxLib.h"

struct MaskData
{
	VECTOR pos;
	int width;
	int height;
	int color;
};


class MaskCreator
{
private:

	//‚©‚Ô‚Á‚Ä‚¢‚é•¨‚ð•`‰æ‚·‚é‚Ì‚©‚»‚ê‚Æ‚à‚©‚Ô‚Á‚Ä‚¢‚é‚à‚ÌˆÈŠO‚©
	bool is_in_ = FALSE;
	bool is_init_ = FALSE;
	MaskCreator();

public:

	static MaskCreator& GetInstance()
	{
		static MaskCreator instance;
		return instance;
	}

	MaskCreator(const MaskCreator&) = delete;
	MaskCreator& operator = (const MaskCreator&) = delete;

	void CreatMask();


	void DeleteMask();


	void Up(const int handle, const bool is_in);


	void Down();
};