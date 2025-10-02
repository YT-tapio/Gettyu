
#include"bat.h"

Bat::Bat()
	:WeaponBase()
{
	model_ = MV1LoadModel("data/model/weapon/use_path/Bat.mv1");
	scale_ = VGet(kScale, kScale, kScale);
	r_ = 3.0f;
	bone_path_ = 0;

	collision_data_.name = CollisionName::kSphere;
	collision_data_.r = 3.0f;
	collision_data_.ver = 0.0f;
	name_ = WeaponName::kBat;
}

Bat::~Bat()
{

}


void Bat::Update(BaseEnemy* enemy)
{

}