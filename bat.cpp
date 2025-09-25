
#include"bat.h"

Bat::Bat()
	:WeaponBase()
{
	model_ = MV1LoadModel("data/model/weapon/use_path/Bat.mv1");
	scale_ = VGet(kScale, kScale, kScale);
	r_ = 3.0f;
	bone_path_ = 0;
}

Bat::~Bat()
{

}


void Bat::Update()
{

}