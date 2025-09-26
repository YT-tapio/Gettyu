#pragma once
#include"collision_data.h"
//#include"player.h"

//struct CollisionData;

enum class WeaponName
{
	kNothing,
	kBat,
	kBugNet,
	kWizardStaff
};


struct Weapondata
{
	WeaponName name;
	TCHAR* bone_path;
	VECTOR scale;
	int model;
};


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

	WeaponName name_;

	//ìñÇΩÇËîªíËÇ™ë∂ç›Ç∑ÇÈÉ{Å[ÉìÇÃà íuÇÃî‘çÜ
	int bone_path_;

	CollisionData collision_data_;

public:

	WeaponBase();


	virtual ~WeaponBase();

	virtual void Update();

	void Draw();


	void SetWeaponName(int name);

	//void SetMatrix(const MATRIX& mat) { mat_ = mat; }
	void SetMatrix(const MATRIX& mat) { mat_ = mat; }


	void SetPos(const VECTOR& pos) { pos_ = pos; }


	void SetLocal(bool flag) { local_ = flag; }

	const WeaponName GetName() const { return name_; }


	const VECTOR GetPos() const { return pos_; }


	const MATRIX GetMatrix() const { return mat_; }

	const CollisionData GetCollisionData() const { return collision_data_; }
};
