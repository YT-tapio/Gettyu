#pragma once

enum class WeaponName
{
	kNothing,
	kBat,
	kBugNet
};


struct Weapondata
{
	WeaponName name;
	TCHAR* bone_path;
	VECTOR scale;
	int model;
};


class Weapon
{
private:


	VECTOR pos_;
	MATRIX mat_;

	VECTOR scale_;

	int model_;

public:

	Weapon(const MATRIX& mat,int model,float scale,const VECTOR& pos)
		: scale_(VGet(scale,scale,scale))
	{
		pos_ = pos;
		model_ = model;
		mat_ = mat;
	}

	~Weapon()
	{

	}


	void Draw();


	//void SetMatrix(const MATRIX& mat) { mat_ = mat; }
	void SetMatrix(const MATRIX& mat) { mat_ = mat; }


	void SetPos(const VECTOR& pos) { pos_ = pos; }


	const VECTOR GetPos() const { return pos_; }


	const MATRIX GetMatrix() const { return mat_; }
};
