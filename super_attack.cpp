
#define _USE_MATH_DEFINES
#include <math.h>

#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"super_attack.h"


SuperAttack::SuperAttack(const VECTOR& pos,const char*  file_path)
	: now_situation_num_(0)
	, pos_(pos)
	, effect_handle_(-1)
	, play_handle_(-1)
	, is_play_(FALSE)
	, play_count_(0.f)
	, max_play_count_(0.f)
	, delta_time_(0.f)
{
	effect_ = new Effect("data/effect/Laser02.efkefc", pos_, VGet(static_cast<float>((M_PI / 180) * -90),
		0.0f, 0.0f),4.0f, 20.0f, FALSE);
}


SuperAttack::~SuperAttack()
{
	delete effect_;
}

void SuperAttack::Init()
{
	now_situation_num_ = 0;
	effect_->Init();
}

void SuperAttack::Update()
{

	if (now_situation_num_ == 1)
	{
		effect_->SetPos(pos_);
		effect_->Play();
	}
	else
	{
		effect_->SetIsPlay(FALSE);
	}

}

void SuperAttack::Draw()
{
	if (effect_handle_ == -1)
	{
		DrawSphere3D(pos_, 10, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}
	else
	{

	}


}

