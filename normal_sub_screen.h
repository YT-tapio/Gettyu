#pragma once
#include"base_sub_screen.h"

class NormalSubScreen : public BaseSubScreen
{
private:


public:

	NormalSubScreen(const VECTOR& pos, const int screen_width,const int screen_height,const int width, const int height, bool alpha, AlphaColorType color_type, const int param);

	~NormalSubScreen() override;

	void Update() override;
};