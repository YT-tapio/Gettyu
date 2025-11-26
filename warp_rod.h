#pragma once


#include"DxLib.h"
#include"weapon_base.h"

class WeaponBase;

class WarpRod : public WeaponBase
{

private:



	//モデルのデータやボーンのパスがあって

	const float kScale = 3.0f;


public:


	/// @brief WeaponBaseにscaleやmodelのデータを入れる
	WarpRod();


	~WarpRod() override;


	void Update(EnemyBase* enemy, const float spin_rad) override;



};