#include"DxLib.h"
#include"object_base.h"
#include"rotated_object.h"
#include"FPS.h"
#include"const_rad.h"

RotatedObject::RotatedObject(const VECTOR& pos, const VECTOR& rot, const VECTOR& scale, const char* path,const float& speed)
	: ObjectBase(pos,rot,scale,path)
	, rotate_speed_(speed)
{

}

RotatedObject::~RotatedObject()
{

}

void RotatedObject::SetDeltaTime()
{
	delta_time_ = FPS::GetInstance().GetDeltaTime();
}

void RotatedObject::Init()
{

}

void RotatedObject::Update()
{
	float speed = rotate_speed_ * delta_time_;

	rot_.y += kOneRad * speed;


	if (rot_.y > kReverceRad)
	{
		rot_.y -= (kReverceRad + kReverceRad);
	}

	if (rot_.y < -kReverceRad)
	{
		rot_.y += (kReverceRad + kReverceRad);
	}

	SetMat();
	MV1SetMatrix(model_, mat_);
}

void RotatedObject::Draw()
{
	MV1DrawModel(model_);
}

void RotatedObject::Debug()
{

}