#include"object_base.h"
#include"FPS.h"

ObjectBase::ObjectBase(const VECTOR& pos, const VECTOR& rot, const VECTOR& scale,const char* path)
	: pos_(pos)
	, rot_(rot)
	, scale_(scale)
	, model_(MV1LoadModel(path))
{
	mat_ = MGetTranslate(pos_);
};


ObjectBase::~ObjectBase()
{
	MV1DeleteModel(model_);
}

void ObjectBase::SetMat()
{
	auto pos_mat	= MGetTranslate(pos_);
	auto rot_mat	= MGetRotY(rot_.y);
	auto scale_mat	= MGetScale(scale_);

	mat_ = MMult(MMult(scale_mat, rot_mat), pos_mat);
}

void ObjectBase::SetDeltaTime()
{
	delta_time_ = FPS::GetInstance().GetDeltaTime();
}

void ObjectBase::Init()
{

}

void ObjectBase::Update()
{

}



void ObjectBase::Draw()
{
	
	MV1DrawModel(model_);

}


