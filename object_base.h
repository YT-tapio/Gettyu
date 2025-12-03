#pragma once
#include"DxLib.h"

enum target_type
{
	target,
	camera
};

class ObjectBase
{
private:


protected:

	VECTOR pos_;
	VECTOR rot_;
	VECTOR scale_;
	MATRIX mat_;

	//モデルのデータ
	int model_;

	float delta_time_;

public:

	

	ObjectBase(const VECTOR& pos, const VECTOR& rot, const VECTOR& scale, const char* path);


	virtual ~ObjectBase();

	virtual void SetDeltaTime();

	virtual void Init();


	virtual void Update();


	virtual void Draw();

	virtual void Debug() {};

	/*-----------------*/

	void SetPos(const VECTOR& position) { pos_ = position; }

	const VECTOR GetPos() const { return pos_; }

	

};