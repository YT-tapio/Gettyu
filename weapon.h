#pragma once


class Weapon
{
private:


	VECTOR pos;
	MATRIX mat_;

	VECTOR scale_;

	int model_;

public:

	Weapon(const MATRIX& mat,int model,float scale)
		: scale_(VGet(scale,scale,scale))
	{
		SetMatrix(mat);
		model_ = model;
	}

	~Weapon()
	{

	}


	void Draw();


	void SetMatrix(const MATRIX& mat) { mat_ = mat; }


	const MATRIX GetMatrix() const { return mat_; }
};
