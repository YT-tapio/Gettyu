#include"normal_enemy.h"
#include"situation.h"


NormalEnemy::NormalEnemy(const char* path,const VECTOR& pos,const VECTOR& scale,const VECTOR& dir,Effect* effect)
	:BaseEnemy(MV1LoadModel(path),pos,scale,dir,effect)
{
	collision_data_.name = CollisionName::kSphere;
	collision_data_.pos = pos;
	collision_data_.r = 3.f;
	collision_data_.ver = 0.0;
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

void NormalEnemy::Update(std::shared_ptr<Player> player, bool& got)
{
	// メモ代わり
	// 捕まるかどうかの処理をするplayer側にthisを送ればよさそうやね
	
	//すでにゲットもしくは、hitしているならこの関数は回さない

	if (!is_get_)
	{
		player->IsHitEnemy(this, got);
	}
	else
	{
		return;
	}

	
	if (Situation::GetInstance().GetSituationName() == SituationName::kGet)
	{
		PlayGetEffect();
	}
	else
	{
		EndGetEffect();
	}

	if (!is_get_)
	{
		VECTOR vel = VGet(0.f, 0.f, 0.f);
		vel = VScale(VNorm(dir_), 2.0f);
		//velocity_ = VScale(vel, delta_time_);

		collision_data_.pos = VAdd(collision_data_.pos, velocity_);

		//pos_ = collision_data_.pos;
		//pos_.y = collision_data_.pos.y - collision_data_.r;
		//当たり判定の位置を更新

	}

	//当たり判定の位置は半径分上げる
	collision_data_.pos = pos_;
	collision_data_.pos.y += collision_data_.r;
	
}
