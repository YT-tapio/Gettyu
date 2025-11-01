#pragma once
#include"DxLib.h"

class Gauss
{
private:

	//‚Ú‚©‚µ‚Ì‹­‚³
	int pixel_width_;
	int param_;

public:

	Gauss(int pixel_width, int param);

	~Gauss();

	void Update(const VECTOR& pos, int width, int height, int handle);

};