#pragma once

#include"DxLib.h"
#include"weapon_base.h"

class WeaponBase;

class WizardStaff : public WeaponBase
{

private:

	//吸い込み範囲
	const float kVacuumRange = 30.f;
	const float kVaccuumSpeed = 10.f;
	//モデルのデータやボーンのパスがあって

	const float kScale = 30.0f;


public:


	/// @brief WeaponBaseにscaleやmodelのデータを入れる
	WizardStaff();


	~WizardStaff() override;


	void Update(EnemyBase* enemy, const float spin_rad) override;



};