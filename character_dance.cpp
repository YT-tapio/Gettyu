//#include<iostream>
#include"object_base.h"
#include"animation.h"
#include"character_dance.h"
#include"FPS.h"


CharacterDance::CharacterDance(const VECTOR& pos, const VECTOR& rot, const VECTOR& scale, const char* path,AnimationData& data)
	:ObjectBase(pos, rot, scale, path)
	,type_(data.type)
{
	animation_ = std::make_shared<Animation>();
	AnimationData anim = data;

	anim.model_handle = model_;
	animation_->Add(anim);
	animation_->Attach(anim.type);		//自分のアニメーションを適応させる
}

CharacterDance::~CharacterDance()
{
	animation_->Detach(type_);
}

void CharacterDance::Setting()
{
	//VECTORたちをmatrixにします
	auto pos_mat		= MGetTranslate(pos_);
	auto rot_mat		= MGetRotY(rot_.y);
	auto scale_mat	= MGetScale(scale_);
	
	mat_ = MMult(MMult(scale_mat, rot_mat), pos_mat);
}

void CharacterDance::SetDeltaTime()
{
	delta_time_ = FPS::GetInstance().GetDeltaTime();
	animation_->SetDeltaTime(delta_time_);
}

void CharacterDance::Init()
{

}

void CharacterDance::Update()
{
	animation_->Update(type_);
	Setting();
	MV1SetMatrix(model_, mat_);
}

void CharacterDance::Draw()
{
	MV1DrawModel(model_);
}

void CharacterDance::Debug()
{

}
