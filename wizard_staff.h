#pragma once

#include"DxLib.h"
#include"weapon_base.h"

class WeaponBase;

class WizardStaff : public WeaponBase
{

private:



	//モデルのデータやボーンのパスがあって

	const float kScale = 5.0f;


public:


	/// @brief WeaponBaseにscaleやmodelのデータを入れる
	WizardStaff();


	~WizardStaff() override;


	void Update() override;



};