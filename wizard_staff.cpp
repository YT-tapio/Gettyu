#include"wizard_staff.h"

WizardStaff::WizardStaff()
	:WeaponBase()
{
	const float kCollRadius = 10.f;

	scale_ = VGet(kScale, kScale, kScale);
	model_ = MV1LoadModel("data/model/weapon/vacuum/vacuum.mv1");
	r_ = kCollRadius;
	bone_path_ = 2;

	collision_data_.name = CollisionName::kSphere;
	collision_data_.r = kCollRadius;
	collision_data_.ver = 0.0f;
	name_ = WeaponName::kWizardStaff;
}

WizardStaff::~WizardStaff()
{
	MV1DeleteModel(model_);
}

void WizardStaff::Update(EnemyBase* enemy, const float spin_rad)
{
	//敵が吸い込み範囲内にいないときは早期リターン
	VECTOR vel = VSub(pos_, enemy->GetPos());

	VECTOR rem_vel = VGet(0.f, 0.f, 0.f);

	if (!IsInRange(vel,kVacuumRange))
	{
		rem_vel = VGet(0.f, 0.f, 0.f);
		enemy->SetVelocity(rem_vel);
		enemy->SetVecuum(FALSE);
		return;
	}
	

	// どのくらいの距離間にいるかを検知する

	float vel_per = (kVacuumRange - VSize(vel)) / kVacuumRange;


	//ここからはその範囲内にいるとき
	//enemyのvelocityに吸収分のvelocityをaddする
	//printfDx("ひっちゅう領域です\n");

	//velを正規化する
	VECTOR norm_vel = VNorm(vel);

	//吸引スピードをかける
	VECTOR vacuum_vel = VScale(norm_vel, (kVaccuumSpeed * vel_per));
	vacuum_vel = VScale(vacuum_vel, delta_time_);

	rem_vel = VAdd(rem_vel, vacuum_vel);

	//vacuum_velをenemyのvelocityにたす
	enemy->AddVelocity(rem_vel);
	enemy->SetVecuum(TRUE);
	// 吸い込みをけいぞくさせるための何かが欲しいし、今吸い込みされている状況とかの
	// 各敵に吸引されているときのvelocityを用意しとく


}