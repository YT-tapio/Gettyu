#include"normal_enemy.h"
#include"situation.h"


NormalEnemy::NormalEnemy(const char* path,const VECTOR& pos,const VECTOR& scale,const VECTOR& dir,Effect* get_effect, Effect* got_effect)
	:BaseEnemy(MV1LoadModel(path),pos,scale,dir,get_effect,got_effect)
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

	velocity_ = VGet(0, 0, 0);

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
		state_->Update(this);
	}

	//当たり判定の位置は半径分上げる
	collision_data_.pos = pos_;
	collision_data_.pos.y += collision_data_.r;
	
}
