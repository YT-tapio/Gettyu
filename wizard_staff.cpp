#include"wizard_staff.h"

WizardStaff::WizardStaff()
	:WeaponBase()
{
	scale_ = VGet(kScale, kScale, kScale);
	model_ = MV1LoadModel("data/model/weapon/use_path/Wizard_Staff.mv1");
	r_ = 0;
	bone_path_ = 0;

	collision_data_.name = CollisionName::kSphere;
	collision_data_.r = 2.5f;
	collision_data_.ver = 0.0f;
	name_ = WeaponName::kWizardStaff;
}

WizardStaff::~WizardStaff()
{
	
}

void WizardStaff::Update(BaseEnemy* enemy)
{
	//敵が吸い込み範囲内にいないときは早期リターン
	VECTOR vel = VSub(pos_, enemy->GetPos());

	static VECTOR rem_vel = VGet(0.f, 0.f, 0.f);

	if (!IsInRange(vel,30.f))
	{
		rem_vel = VGet(0.f, 0.f, 0.f);
		enemy->SetVelocity(rem_vel);
		return;
	}

	//ここからはその範囲内にいるとき
	//enemyのvelocityに吸収分のvelocityをaddする
	//printfDx("ひっちゅう領域です\n");


	//velを正規化する
	VECTOR norm_vel = VNorm(vel);

	//吸引スピードをかける
	VECTOR vacuum_vel = VScale(norm_vel, 1.1f);
	vacuum_vel = VScale(vacuum_vel, delta_time_);

	rem_vel = VAdd(rem_vel, vacuum_vel);

	//vacuum_velをenemyのvelocityにたす
	enemy->SetVelocity(rem_vel);


	// 吸い込みをけいぞくさせるための何かが欲しいし、今吸い込みされている状況とかの
	// 


}