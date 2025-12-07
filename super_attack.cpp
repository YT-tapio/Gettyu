
#define _USE_MATH_DEFINES
#include <math.h>

#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"super_attack.h"
#include"super_attack_cool_time.h"
#include"input.h"
#include"keyconfig.h"

SuperAttack::SuperAttack(const VECTOR& pos,const char*  file_path)
	: now_situation_num_(0)
	, effect_end_pos_(VGet(0.f,0.f,0.f))
	, effect_pos_(pos)
	, effect_handle_(-1)
	, play_handle_(-1)
	, is_play_(FALSE)
	, is_ready_(FALSE)
	, is_active_(FALSE)
	, is_offset_(FALSE)
	, delta_time_(0.f)
	, skill_num_(0.f)
{
	effect_ = new Effect("data/effect/Effekseer01/Laser02.efkefc", effect_pos_, VGet(static_cast<float>((M_PI / 180) * -90),
		0.0f, 0.0f),7.0f,5.0f, 200.0f, FALSE);

	effect_start_ = new Effect("data/effect/NextSoft01/MagicTornade.efkefc", effect_pos_, VGet(0.0f,
		0.0f, 0.0f), 7.0f, 5.0f, 300.0f, FALSE);

	effect_end_ = new Effect("data/effect/Pierre01/Flame.efkefc", effect_pos_, VGet(0.0f,
		0.0f, 0.0f), 7.5f, 5.0f, 200.0f, FALSE);

	state_ = SuperAttackState::kCoolTime;

	// conditiontimer‚Ìsetup
	cool_time_		= std::make_shared<ConditionTimer>(kCoolTimeMax);
	active_time_	= std::make_shared<ConditionTimer>(kActiveTimeMax);
	offset_time_	= std::make_shared<ConditionTimer>(kOffsetTimeMax);


}


SuperAttack::~SuperAttack()
{
	delete effect_;
	delete effect_start_;
	delete effect_end_;
}

void SuperAttack::CoolTimeUpdate()
{
	cool_time_->Update();
	skill_num_ = cool_time_->GetTimeRatio();
	if (cool_time_->GetIsEnd())
	{
		cool_time_->Reset();
		state_ = SuperAttackState::kReady;
	}
}

void SuperAttack::OffsetUpdate()
{
	offset_time_->Update();

	if (offset_time_->GetIsEnd())
	{
		offset_time_->Reset();
		state_ = SuperAttackState::kActive;
	}

}

void SuperAttack::ActiveUpdate()
{
	active_time_->Update();
	float reverce_num = 1 - active_time_->GetTimeRatio();
	skill_num_ = reverce_num;
	if (active_time_->GetIsEnd()) 
	{
		active_time_->Reset();
		state_ = SuperAttackState::kCoolTime;
	}
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

	switch (state_)
	{
	case SuperAttackState::kCoolTime:
		CoolTimeUpdate();
		break;

	case SuperAttackState::kOffset:
		OffsetUpdate();
		break;

	case SuperAttackState::kActive:
		ActiveUpdate();
		break;
	}

	SuperAttackCoolTime::GetInstance().SetRatio(skill_num_);
	
}

void SuperAttack::EffectUpdate()
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
	cool_time_->Reset();
}

void SuperAttack::Draw()
{
	if (effect_handle_ == -1)
	{
		//DrawSphere3D(effect_pos_, 10, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}
}


void SuperAttack::Debug()
{
	cool_time_->Debug();
}

bool SuperAttack::IsAction()
{
	if (state_ != SuperAttackState::kReady) { return FALSE; }

	if (Input::GetInstance().CheckInputMouse(KeyConfig::kSuperAttackKey) == InputState::kPush ||
		Input::GetInstance().CheckInputPadButton(PadConfig::kSuperAttackButton) == InputState::kPush)
	{
		state_ = SuperAttackState::kOffset;
		return TRUE;
	}

	return FALSE;
}
