#define _USE_MATH_DEFINES
#include <math.h>
#include"DxLib.h"
#include"dance_enemy.h"
#include"lerp.h"
#include"vector_assistant.h"
#include"const_rad.h"

DanceEnemy::DanceEnemy(const TCHAR* model_path, const VECTOR& pos,
	const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect,
	float speed, float fleeping_speed, AlertState alert, float fov, std::shared_ptr<Stage> stage, Navigation* navigation)
	: NormalEnemy(model_path,pos,scale,dir,get_effect,got_effect,speed,fleeping_speed,alert,fov,stage,navigation)
	,is_contact_(FALSE)
{

}

DanceEnemy::~DanceEnemy()
{

}

void DanceEnemy::Init(const VECTOR& pos, const VECTOR scale)
{

}

void DanceEnemy::AddAnim()
{
	//アニメーションスピード
	const float kAnimationWalkSpeed = 3.f;
	const float kAnimationSurpriseSpeed = 10.f;
	const float kAnimationFastRunSpeed = 5.f;
	const float kAnimationIdleSpeed = 4.f;
	const float kAnimationAlertSpeed = 1.f;
	const float kAnimationStanSpeed = 2.f;
	const float kAnimationDanceSpeed = 3.f;

	const char* kAnimationIdlePath = "data/model/character/enemy/animation/Standing_W_Briefcase_Idle.mv1";
	const char* kAnimationWalkPath = "data/model/character/enemy/animation/Walking.mv1";
	const char* kAnimationSurprisePath = "data/model/character/enemy/animation/Joyful_Jump.mv1";
	const char* kAnimationFastRunPath = "data/model/character/enemy/animation/Standard_Run.mv1";
	const char* kAnimationAlertPath = "data/model/character/enemy/animation/Standing_Cover_Turn.mv1";
	const char* kAnimationStanPath = "data/model/character/enemy/animation/Female_Dynamic_Pose.mv1";
	const char* kAnimationDancePath = "data/model/character/enemy/animation/Female_Dynamic_Pose.mv1";

	//各アニメーションを生成する
	AnimationData idle;
	AnimationData walk;
	AnimationData surprise;
	AnimationData fast_run;
	AnimationData alert;
	AnimationData stan;
	AnimationData dance;

	//アニメーションをロード
	Load(idle, kAnimationIdlePath,			AnimationType::kIdle, model_, 1, kAnimationIdleSpeed);
	Load(walk, kAnimationWalkPath,			AnimationType::kWalk, model_, 1, kAnimationWalkSpeed);
	Load(surprise, kAnimationSurprisePath,	AnimationType::kSurprise, model_, 1, kAnimationSurpriseSpeed);
	Load(fast_run, kAnimationFastRunPath,	AnimationType::kFastRun, model_, 1, kAnimationFastRunSpeed);
	Load(alert, kAnimationAlertPath,		AnimationType::kAlert, model_, 1, kAnimationAlertSpeed);
	Load(stan, kAnimationStanPath,			AnimationType::kStan, model_, 1, kAnimationStanSpeed);
	Load(dance, kAnimationDancePath,		AnimationType::kDancing, model_, 1, kAnimationDanceSpeed);

	animation_->Add(idle);
	animation_->Add(walk);
	animation_->Add(surprise);
	animation_->Add(fast_run);
	animation_->Add(alert);
	animation_->Add(stan);
	animation_->Add(dance);
}

void DanceEnemy::PatrollingInit(std::shared_ptr<Player> player)
{
	// ここでアニメーションをきりかえます
	//if()
}

void DanceEnemy::Patrolling()
{
	// 最初はダンスさせる

	if (!is_contact_)
	{
		now_anim_type_ = AnimationType::kDancing;

		return;
	}

	// それ以降は普通のネズミになる

	VECTOR vel = VGet(0, 0, 0);

	//線形保管でよくね
	//velにlerpのやつを代入
	if (lerp_flag_)
	{
		now_anim_type_ = ChageAnimType(now_anim_type_, AnimationType::kWalk);
		vel = NormalLerp(pos_, target_pos_, (speed_ * delta_time_), lerp_flag_);
	}
	else
	{
		//タイマーが終了したら
		if (wait_timer_->GetIsEnd())
		{
			// ここで次の場所を指定する
			DecideNextPos();
		}
		else
		{
			//lerpし終わったらwait_timerをきどうしてそれが終わったら
			now_anim_type_ = ChageAnimType(now_anim_type_, AnimationType::kIdle);
			wait_timer_->Update();
		}
	}

	velocity_ = VAdd(velocity_, vel);
	total_vel_ = VAdd(total_vel_, vel);
	if (VSize(velocity_) > 0) { dir_ = VNorm(velocity_); }


	rot_.y = VectorAssistant::GetPlaneRad(dir_);

	rot_.y += kReverceRad;
	if (rot_.y > kReverceRad)
	{
		rot_.y -= (kReverceRad + kReverceRad);
	}


}


void DanceEnemy::PatrollingExit()
{
	is_contact_ = TRUE;
}