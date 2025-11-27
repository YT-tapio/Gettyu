#include"object_base.h"
#include"FPS.h"

ObjectBase::ObjectBase(const VECTOR& pos, const VECTOR& rot, const VECTOR& scale,int model_handle)
	: pos_(pos)
	, rot_(rot)
	, scale_(scale)
	, model_(model_handle)
{
	mat_ = MGetTranslate(pos_);
};


ObjectBase::~ObjectBase()
{
	MV1DeleteModel(model_);
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


