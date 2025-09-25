
#define _USE_MATH_DEFINES
#include <math.h>

#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"super_attack.h"


SuperAttack::SuperAttack(const VECTOR& pos,const char*  file_path)
	: now_situation_num_(0)
	, effect_end_pos_(VGet(0.f,0.f,0.f))
	, effect_pos_(pos)
	, effect_handle_(-1)
	, play_handle_(-1)
	, is_play_(FALSE)
	, play_count_(0.f)
	, max_play_count_(0.f)
	, delta_time_(0.f)
{
	effect_ = new Effect("data/effect/Effekseer01/Laser02.efkefc", effect_pos_, VGet(static_cast<float>((M_PI / 180) * -90),
		0.0f, 0.0f),5.0f, 20.0f, FALSE);

	effect_start_ = new Effect("data/effect/NextSoft01/MagicTornade.efkefc", effect_pos_, VGet(0.0f,
		0.0f, 0.0f), 5.0f, 20.0f, FALSE);

	effect_end_ = new Effect("data/effect/Pierre01/Flame.efkefc", effect_pos_, VGet(0.0f,
		0.0f, 0.0f), 5.0f, 20.0f, FALSE);

}


SuperAttack::~SuperAttack()
{
	delete effect_;
	delete effect_start_;
	delete effect_end_;
}

void SuperAttack::Init()
{
	now_situation_num_ = 0;
	effect_->Init();
	effect_start_->Init();
	effect_end_->Init();
}

void SuperAttack::Update()
{

	if (now_situation_num_ == 1)
	{
		effect_->SetPos(effect_pos_);
		effect_->Play();
	}
	else
	{
		effect_->SetIsPlay(FALSE);
	}


	if (now_situation_num_ == 0)
	{
		effect_start_->SetPos(effect_end_pos_);
		effect_start_->Play();
	}
	else
	{
		effect_start_->SetIsPlay(FALSE);
	}

	if (now_situation_num_ == 1)
	{
		effect_end_->SetPos(effect_end_pos_);
		effect_end_->Play();
	}
	else
	{
		effect_end_->SetIsPlay(FALSE);
	}



	//printfDx("%.2f\n",effect_end_->GetPlayCount());

}

void SuperAttack::Draw()
{
	if (effect_handle_ == -1)
	{
		//DrawSphere3D(effect_pos_, 10, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}

}

