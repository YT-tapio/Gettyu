#include<iostream>
#include"DxLib.h"
#include"base_object.h"
#include"still_object.h"


StillObject::StillObject(VECTOR position, int model_handle)
	: BaseObject(position,model_handle)
{

}


StillObject::~StillObject()
{

}


void StillObject::Init(VECTOR position)
{
	position_ = position;
}


void StillObject::Update()
{

}


void StillObject::Draw()
{
	if (!(model_ == -1))
	{
		//行列を生成
		MATRIX pos_matrix = MGetTranslate(position_);

		//モデルの行列をセットする
		matrix_ = pos_matrix;

		MV1SetMatrix(model_, matrix_);
		MV1DrawModel(model_);
	}
	else
	{
		DrawSphere3D(position_, 3, 3, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}
	
}