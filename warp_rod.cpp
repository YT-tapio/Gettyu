#include"warp_rod.h"

WarpRod::WarpRod()
	:WeaponBase()
{
	scale_ = VGet(kScale, kScale, kScale);
	model_ = MV1LoadModel("data/model/weapon/use_path/Bug_Net3.mv1");
	r_ = 3.0f;
	bone_path_ = 4;


	collision_data_.name = CollisionName::kSphere;
	collision_data_.r = 5.0f;
	collision_data_.ver = 0.0f;
	name_ = WeaponName::kBugNet;
}

WarpRod::~WarpRod()
{

}

void WarpRod::Update(EnemyBase* enemy, const float spin_rad)
{
	

}