
#define _USE_MATH_DEFINES
#include <math.h>

#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"super_attack.h"
#include"super_attack_cool_time.h"
#include"input.h"
#include"keyconfig.h"
#include"situation.h"
#include"sound.h"
#include"2D_sound.h"
#include"clear_time.h"
#include"super_attack_state_getter.h"
#include"debug.h"

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

	// conditiontimerのsetup
	cool_time_		= std::make_shared<ConditionTimer>(kCoolTimeMax);
	active_time_	= std::make_shared<ConditionTimer>(kActiveTimeMax);
	offset_time_	= std::make_shared<ConditionTimer>(kOffsetTimeMax);

	const char* kReadySoundPath = "data/sound/game/se/super_attack/ready_sound.mp3";
	const char* kThunderSoundPath = "data/sound/game/se/super_attack/thunder.MP3";
	const char* kBombSoundPath = "data/sound/game/se/super_attack/bomb.mp3";

	effect_sound_ = std::make_shared<Sound2D>(kReadySoundPath, DX_PLAYTYPE_BACK, 100, FALSE);
	thunder_sound_ = std::make_shared<Sound2D>(kThunderSoundPath, DX_PLAYTYPE_BACK, 100, FALSE);
	bomb_sound_ = std::make_shared<Sound2D>(kBombSoundPath, DX_PLAYTYPE_BACK, 200, FALSE);
	
	SuperAttackStateGetter::GetInstance().Set(&state_);
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
		//サウンド関連をreset
		effect_sound_->Reset();
		thunder_sound_->Reset();
		bomb_sound_->Reset();

		cool_time_->Reset();
		state_ = SuperAttackState::kReady;
	}
}

void SuperAttack::OffsetUpdate()
{
	ClearTime::GetInstance().Stop();
	offset_time_->Update();
	
	Situation::GetInstance().SetSituationName(SituationName::kPerformance);
	if (offset_time_->GetIsEnd())
	{
		ClearTime::GetInstance().Start();
		Situation::GetInstance().SetSituationName(SituationName::kSuperAttack);
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

void SuperAttack::TimerStop()
{
	cool_time_->Stop();
	active_time_->Stop();
	offset_time_->Stop();
}

void SuperAttack::TimerStart()
{
	cool_time_->Start();
	active_time_->Start();
	offset_time_->Start();
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

	if (Situation::GetInstance().GetSituationName() == SituationName::kStandBy)
	{
		TimerStop();
	}
	else
	{
		TimerStart();
	}


	// デバッグ時タイマーを直接敵にmaxへ
	if (Debug::GetInstance().GetDisp())
	{
		if (Input::GetInstance().CheckInputKey(KEY_INPUT_P) == InputState::kPush)
		{
			cool_time_->Max();
		}
	}

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

	const float kBombSoundPlayTiming = 100.f;

	if (now_situation_num_ == 1)
	{
		effect_->SetPos(effect_pos_);
		effect_->Play();
		thunder_sound_->Update();
	}
	else
	{
		effect_->SetIsPlay(FALSE);
	}


	if (now_situation_num_ == 0)
	{
		effect_start_->SetPos(effect_end_pos_);
		effect_start_->Play();
		effect_sound_->Update();
	}
	else
	{
		effect_start_->SetIsPlay(FALSE);
	}

	if (now_situation_num_ == 1)
	{
		effect_end_->SetPos(effect_end_pos_);
		effect_end_->Play();

		if (effect_end_->GetPlayCount() >= kBombSoundPlayTiming)
		{
			bomb_sound_->Update();
		}
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
