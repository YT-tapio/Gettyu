
#define _USE_MATH_DEFINES
#include <math.h>


#include"normal_enemy.h"
#include"situation.h"
#include"rot_function.h"

NormalEnemy::NormalEnemy(const char* path,const VECTOR& pos,const VECTOR& scale,const VECTOR& dir,Effect* get_effect, Effect* got_effect, float speed, float alert_dist, float fov)
	:BaseEnemy(MV1LoadModel(path),pos,scale,dir,get_effect,got_effect,speed,alert_dist,fov)
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


void NormalEnemy::FleepingInit(std::shared_ptr<Player> player)
{
	//ここでrotを指定してdirも指定する。
	//とりあえず反転して逃げさせる
	rot_.y += kReverceRad;
	dir_ = VGet(-sinf(rot_.y), 0.f, -cosf(rot_.y));

}

void NormalEnemy::Update(std::shared_ptr<Player> player, bool& got)
{

	velocity_ = VGet(0, 0, 0);
	
	//状態変化
	const auto next_state = fsm_->UpdateState(state_, player, this);

	if (state_->GetName() != next_state->GetName())
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

	if (!is_get_)
	{
		//stateによるupdate
		state_->Update(this,player);
	}

	//ここで当たり判定を行う


	//ポジションの更新

	pos_ = VAdd(pos_, velocity_);

	//当たり判定の位置は半径分上げる
	collision_data_.pos = pos_;
	collision_data_.pos.y += collision_data_.r;
	
}


void NormalEnemy::Patrolling()
{
	// この中で散歩させておく
	// パトロールの方法も変える
	
	//最大値(移動量)
	const float kMaxVel = 20.f;
	//回転のオフセット時間
	const float kOffsetTime = 1.f;
	
	VECTOR vel = VGet(0, 0, 0);

	//どんだけ歩いているかの確認をする
	//今までの歩いてきた量を保存

	dir_ = VGet(-sinf(rot_.y), 0.f, -cosf(rot_.y));
	vel = VScale(dir_, speed_);
	vel = VScale(vel, delta_time_);
	
	//ここで判断してあげる
	if (VSize(total_vel_) >= kMaxVel)
	{
		//初期化と反転を行う
		total_vel_ = VGet(0, 0, 0);
		//rotの反転
		target_rot_ = rot_.y + kReverceRad;

		
		if (target_rot_ >= kReverceRad)
		{
			target_rot_ -= (kReverceRad + kReverceRad);
		}

		
		
		is_return_ = TRUE;
	}


	/*
	

	
	*/


	if (is_return_)
	{
		if (rot_.y != target_rot_)
		{
			CheckReverseRotFunc(rot_.y, target_rot_, delta_time_, kOffsetTime);
		}
		else
		{
			is_return_ = FALSE;
		}
		vel = VGet(0.f, 0.f, 0.f);
		
	}
	velocity_ = VAdd(velocity_, vel);
	total_vel_ = VAdd(total_vel_,vel);
}


void NormalEnemy::Alert(std::shared_ptr<Player> player)
{
	//ここでplayerとの距離を測りどんな状態に変化するか判断




}



void NormalEnemy::Fleeping(std::shared_ptr<Player> player)
{
	//ここで逃げる

	// どう逃げさせようかな
	// 一定距離うごいたら初期化させexitさせていいと思う

	velocity_ = VScale(dir_, 3.f);


}