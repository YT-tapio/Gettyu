
#define _USE_MATH_DEFINES
#include <math.h>


#include"normal_enemy.h"
#include"situation.h"
#include"rot_function.h"

NormalEnemy::NormalEnemy(const TCHAR* model_path, const VECTOR& pos, const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect, float speed, float fleeping_speed, AlertState alert, float fov)
	:BaseEnemy(MV1LoadModel(model_path),pos,scale,dir,get_effect,got_effect,speed,fleeping_speed,alert,fov)
{
	collision_data_.name = CollisionName::kSphere;
	collision_data_.pos = pos;
	collision_data_.r = 3.f;
	collision_data_.ver = 0.0;
	total_vel_ = VGet(0, 0, 0);
	//反転するときの値
	target_rot_ = 0.f;
	is_return_ = FALSE;
}

NormalEnemy::~NormalEnemy()
{

}


void NormalEnemy::Init(const VECTOR& pos, const VECTOR scale)
{
	pos_ = pos;
	dir_ = VGet(0.f, 0.f, 0.f);
	velocity_ = VGet(0.f, 0.f, 0.f);
	scale_ = scale;

	is_get_ = FALSE;
	delta_time_ = 0.0f;
}

void NormalEnemy::PatrollingInit(std::shared_ptr<Player> player)
{
	total_vel_ = VGet(0, 0, 0);
	is_return_ = FALSE;
	target_rot_ = rot_.y;
	is_fleeping_ = FALSE;

	

	// ここでどのwaypointに向かわせるかの判定を行う
	// どんな関数を用意する?
	// どこに行くかを決めて、dirを返してくれる関数を用意する
	dir_ = DecideNextPlace();

	// 線形保管で移動するのでposを保存
	start_pos_ = pos_;
	now_anim_type_ = AnimationType::kWalk;
}


void NormalEnemy::SurpriseInit(std::shared_ptr<Player> player)
{
	//プレイヤーの方向を向く
	//playerと敵の距離を見る

	VECTOR dist = VSub(player->GetPos(), pos_);

	rot_.y = atan2f(dist.x, dist.z);
	//プレイヤーの方向を見させる
	rot_.y += kReverceRad;

	if (rot_.y >= kReverceRad)
	{
		rot_.y -= (kReverceRad + kReverceRad);
	}

	//アニメーションも変化させる
	now_anim_type_ = AnimationType::kSurprise;
}


void NormalEnemy::AlertInit(std::shared_ptr<Player> player)
{
	//タイマーのリセット
	alert_timer_->Reset();
	is_alert_ = TRUE;
	//独自のアニメーションも再生させたい(探しているような)
	now_anim_type_ = AnimationType::kNothing;
}


void NormalEnemy::FleepingInit(std::shared_ptr<Player> player)
{
	//ここでrotを指定してdirも指定する。
	//とりあえず反転して逃げさせる。

	//playerとenemyのposで逃げるのを指定

	VECTOR enemy_to_player_dist = VSub(player->GetCenterPos(), pos_);

	VECTOR norm_dist = VNorm(enemy_to_player_dist);

	rot_.y = atan2f(norm_dist.x,norm_dist.z);

	dir_ = VGet(-sinf(rot_.y), 0.f, -cosf(rot_.y));
	total_vel_ = VGet(0, 0, 0);

	is_fleeping_ = TRUE;
	now_anim_type_ = AnimationType::kFastRun;
}


void NormalEnemy::AddAnim()
{
	//アニメーションスピード
	const float kAnimationWalkSpeed = 3.f;
	const float kAnimationSurpriseSpeed = 10.f;
	const float kAnimationFastRunSpeed = 5.f;
	//各アニメーションを生成する

	AnimationData walk;
	AnimationData surprise;
	AnimationData fast_run;
	
	//アニメーションをロード
	Load(walk, "data/model/character/enemy/animation/Walking.mv1", AnimationType::kWalk, model_, 1, kAnimationWalkSpeed);
	Load(surprise, "data/model/character/enemy/animation/Joyful_Jump.mv1", AnimationType::kSurprise, model_, 1, kAnimationSurpriseSpeed);
	Load(fast_run, "data/model/character/enemy/animation/Standard_Run.mv1", AnimationType::kFastRun, model_, 1, kAnimationFastRunSpeed);

	animation_->Add(walk);
	animation_->Add(surprise);
	animation_->Add(fast_run);
}


void NormalEnemy::Update(std::shared_ptr<Player> player, bool& got)
{

	velocity_ = VGet(0, 0, 0);
	
	//状態変化
	const auto next_state = fsm_->UpdateState(state_, player, this);


	if (state_ != nullptr)
	{
		if (state_->GetName() != next_state->GetName())
		{
			state_->Exit(this);
			state_ = next_state;
			state_->Entry(this, player);
		}
	}
	else
	{
		state_ = next_state;
		state_->Entry(this, player);
	}
	

	// メモ代わり
	// 捕まるかどうかの処理をするplayer側にthisを送ればよさそうやね
	
	//すでにゲットもしくは、hitしているならこの関数は回さない

	if (!is_get_)
	{
		//ここでまだ捕まっていないときは
		player->IsHitEnemy(this, got);
	}
	else
	{
		return;
	}

	//stateによるupdate
	state_->Update(this, player);

	//アニメーションの更新
	AnimationUpdate();

	//ポジションの更新

	pos_ = VAdd(pos_, velocity_);

	//当たり判定の位置は半径分上げる
	collision_data_.pos = pos_;
	collision_data_.pos.y += collision_data_.r;
	
}


void NormalEnemy::Patrolling()
{
	VECTOR vel = VGet(0,0,0);

	//線形保管でよくね

	vel = VScale(dir_, speed_);

	vel = VScale(vel, delta_time_);

	velocity_ = VAdd(velocity_, vel);
	total_vel_ = VAdd(total_vel_,vel);

	rot_.y = atan2f(-dir_.x , -dir_.z);
	
	// ここでway_pointのcheckを行う
	// my_way_pointから知っているneighborsに向かわせる
	// my_way_point付近にいるのを感知する関数を用意

	

}


void NormalEnemy::Surprise()
{

}


void NormalEnemy::Alert(std::shared_ptr<Player> player)
{
	//ここでplayerとの距離を測りどんな状態に変化するか判断
	alert_timer_->Update();
	//タイマーが終了したら
	if (alert_timer_->GetIsEnd())
	{
		//アラートを解除
		is_alert_ = FALSE;
	}

}



void NormalEnemy::Fleeping(std::shared_ptr<Player> player)
{

	//定数
	const float kFleepingMax = 30.f;

	//ここで逃げる

	// どう逃げさせようかな
	// 一定距離うごいたら初期化させexitさせていいと思う

	VECTOR vel = VGet(0.f,0.f,0.f);

	vel = VScale(dir_, fleeping_speed_);

	velocity_ = VAdd(velocity_,VScale(vel, delta_time_));

	total_vel_ = VAdd(total_vel_,velocity_);

	//ここでtotal_vel_がまだ逃げ切ってないときは
	if (VSize(total_vel_) > kFleepingMax)
	{
		total_vel_ = VGet(0, 0, 0);
		is_fleeping_ = FALSE;
	}

}