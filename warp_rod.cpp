#include"warp_rod.h"

WarpRod::WarpRod()
	:WeaponBase()
{
	scale_ = VGet(kScale, kScale, kScale);
	model_ = MV1LoadModel("data/model/weapon/use_path/Bug_Net3.mv1");
	r_ = 3.0f;
	bone_path_ = 4;
}

WarpRod::~WarpRod()
{

}

void WarpRod::Update()
{

}