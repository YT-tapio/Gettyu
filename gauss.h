#pragma once
#include"DxLib.h"

class Gauss
{
private:

	Gauss();
public:

	static Gauss& GetInstance()
	{
		static Gauss instance;
		return instance;
	}

	Gauss(const Gauss&) = delete;
	Gauss& operator = (const Gauss&) = delete;

	void Update(const VECTOR& pos, int width, int height, int handle, int pixel_width, int param);

};