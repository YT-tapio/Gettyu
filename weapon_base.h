#pragma once
#include<iostream>
#include<vector>
#include<list>

#include"collision_data.h"
#include"base_enemy.h"
//#include"player.h"

//struct CollisionData;

enum class WeaponName
{
	kNothing,
	kBat,
	kBugNet,
	kWizardStaff,
	kSuperAttack
};


struct Weapondata
{
	WeaponName name;
	TCHAR* bone_path;
	VECTOR scale;
	int model;
};

//class BaseEnemy;


class WeaponBase
{
private:

	

protected:

	int model_;

	VECTOR pos_;
	MATRIX mat_;

	VECTOR scale_;
	VECTOR velocity_;

	bool local_;

	float r_;
	float delta_time_;

	WeaponName name_;

	//ìñÇΩÇËîªíËÇ™ë∂ç›Ç∑ÇÈÉ{Å[ÉìÇÃà íuÇÃî‘çÜ
	int bone_path_;

	CollisionData collision_data_;

	std::vector<VECTOR> rem_poss_;


public:

	WeaponBase();


	virtual ~WeaponBase();

	virtual void Update(BaseEnemy* enemy,const float spin_rad);

	/// <summary>
	/// îÕàÕì‡Ç…Ç¢ÇÈÇ∆Ç´
	/// </summary>
	bool IsInRange(const VECTOR& vel,float range);

	void Draw(float delta_time);


	void SetWeaponName(int name);

	void SetDeltaTime(float delta_time);

	//void SetMatrix(const MATRIX& mat) { mat_ = mat; }
	void SetMatrix(const MATRIX& mat) { mat_ = mat; }


	void SetModelMatrix(const MATRIX& mat) { MV1SetMatrix(model_, MMult(MGetScale(scale_), mat)); }

	void SetPos(const VECTOR& pos) { pos_ = pos; }


	void SetLocal(bool flag) { local_ = flag; }


	const WeaponName GetName() const { return name_; }


	const VECTOR GetPos() const { return pos_; }


	const MATRIX GetMatrix() const { return mat_; }

	const CollisionData GetCollisionData() const { return collision_data_; }
};
