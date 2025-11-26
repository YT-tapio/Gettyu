#pragma once
#include"DxLib.h"
#include"weapon_base.h"

class WeaponBase;

class Bat : public WeaponBase
{

private:

	

	//モデルのデータやボーンのパスがあって
	const float kScale = 8.0f;


public:


	/// @brief WeaponBaseにscaleやmodelのデータを入れる
	Bat();


	~Bat() override;


	void Update(EnemyBase* enemy, const float spin_rad) override;



};


//ご飯つくります
