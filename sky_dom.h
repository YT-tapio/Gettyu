#pragma once
#include"DxLib.h"

class SkyDom
{
private:

	VECTOR pos_;
	int model_data_;

public:

	SkyDom(const char* path,const VECTOR& pos);

	~SkyDom();



	void Draw();


	void SetPos(const VECTOR vel) { pos_ = VAdd(pos_, vel); MV1SetPosition(model_data_, pos_); }
};