
#include"base_enemy.h"


BaseEnemy::BaseEnemy(const int model, const VECTOR& pos,
	const VECTOR& scale, const VECTOR& dir)
{
	//モデルのダウンロード
	model_ = model;
	if (model_ == -1)
	{
		//printfDx("enemyのモデル読み込み失敗\n");
	}

	//VECTOR
	pos_ = pos;
	dir_ = VGet(0.f, 0.f, 0.f);
	rot_ = VGet(0.f, 0.f, 0.f);
	velocity_ = VGet(0.f, 0.f, 0.f);
	scale_ = scale;
	dir_ = dir;
	mat_ = MMult(MMult(MGetRotY(0.0f), MGetScale(scale_)), 
		MGetTranslate(pos_));

	is_get_ = FALSE;
	delta_time_ = 0.0f;


}

BaseEnemy::~BaseEnemy()
{

}


void BaseEnemy::Draw(int i)
{
	if (!is_get_)
	{
		
	}


	mat_ = MMult(MGetScale(scale_), MGetTranslate(pos_));

	

	switch (collision_data_.name)
	{

	case CollisionName::kSphere:

		DrawSphere3D(collision_data_.pos, 3.f, 15, GetColor(100 * (i), 255 - (70 * i), 100 - (0 * i)),
			GetColor(50 * (i), 255 - (50 * i), 255), FALSE);

		break;

	case CollisionName::kCapsule:

		DrawCapsule3D(collision_data_.pos, VGet(collision_data_.pos.x, (collision_data_.pos.y - collision_data_.ver),
			collision_data_.pos.z), collision_data_.r, 15, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		break;
	}

	if (model_ == -1)
	{

		


	}
	else
	{
		MV1SetMatrix(model_, mat_);
		MV1DrawModel(model_);
	}
	//いろいろなデバッグの作業をしていきます
	//キャラクターの向いているところを表示
	//dirに準ずる

	DrawLine3D(pos_, VAdd(pos_, dir_), GetColor(255, 255, 255));

	//座標表示
	DrawFormatString(0, 15 + (15 * i), GetColor(100 * (i), 255 - (50 * i), 100 - (0 * i)), "enemy%d_collision_pos:: x:%.2f,x:%.2f,x:%.2f", i, collision_data_.pos.x,
		collision_data_.pos.y, collision_data_.pos.z);
	
}

void BaseEnemy::SetDeltaTime(float delta_time)
{
	delta_time_ = delta_time;
}

void BaseEnemy::SetIsGet(bool flag)
{
	is_get_ = flag;
}

void BaseEnemy::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

void BaseEnemy::SetPosIsGot(const VECTOR& pos)
{
	pos_ = pos;
	pos_.y -= collision_data_.r;
}