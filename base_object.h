#pragma once
#include"DxLib.h"

enum target_type
{
	target,
	camera
};

class BaseObject
{
private:



protected:

	VECTOR position_;
	MATRIX matrix_;


	//モデルのデータ
	int model_;

public:

	

	BaseObject(VECTOR position,int model_handle)
		: position_(position)
		, model_(model_handle)
		,matrix_(MGetTranslate(position_))
	{

	};

	virtual ~BaseObject() {};


	virtual void Init(VECTOR position) {};


	virtual void Update() {};


	virtual void Draw() {};



	/*-----------------*/

	void SetPos(const VECTOR& position) { position_ = position; }

	const VECTOR GetPos() const { return position_; }

	

};