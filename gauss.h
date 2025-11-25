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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="handle"></param>
	/// <param name="pixel_width"></param>
	/// <param name="param"></param>
	void Update(int handle, int pixel_width, int param);

};