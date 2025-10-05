
#include"base_enemy.h"
#include"situation.h"

BaseEnemy::BaseEnemy(const int model, const VECTOR& pos,
	const VECTOR& scale, const VECTOR& dir, Effect* get_effect,Effect* got_effect)
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

	get_effect_ = get_effect;
	got_effect_ = got_effect;
}

BaseEnemy::~BaseEnemy()
{
	//delete get_effect_;
	//delete got_effect_;
}


void BaseEnemy::EffectUpdate()
{
	if (Situation::GetInstance().GetSituationName() == SituationName::kGet)
	{
		got_effect_->End();
		PlayGetEffect();
	}
	else
	{
		EndGetEffect();
		//すでにゲットされているなら
		if (is_get_)
		{
			got_effect_->Play();			
		}
	}

	

}

void BaseEnemy::PlayGetEffect()
{
	get_effect_->Play();
}


void BaseEnemy::EndGetEffect()
{
	get_effect_->End();
}


void BaseEnemy::Draw(int i)
{

	mat_ = MMult(MGetScale(scale_), MGetTranslate(pos_));

	if (FALSE)
	{
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

	

	//座標表示
	if (FALSE)
	{
		DrawLine3D(pos_, VAdd(pos_, dir_), GetColor(255, 255, 255));
		DrawFormatString(0, 15 + (15 * i), GetColor(100 * (i), 255 - (50 * i), 100 - (0 * i)), "enemy%d_collision_pos:: x:%.2f,y:%.2f,z:%.2f", i, collision_data_.pos.x,
			collision_data_.pos.y, collision_data_.pos.z);
	}

}

void BaseEnemy::Debug()
{
	//でばっくのシングルトンから今までのデバックのログ数を受け取りその量を受け取る
}

void BaseEnemy::SetDeltaTime(float delta_time)
{
	delta_time_ = delta_time;
	get_effect_->SetDeltaTime(delta_time);
	got_effect_->SetDeltaTime(delta_time);
}

void BaseEnemy::SetIsGet(bool flag)
{
	is_get_ = flag;
}

void BaseEnemy::SetVelocity(const VECTOR& vel)
{
	velocity_ = vel;
}

void BaseEnemy::AddVelocity(const VECTOR& vel)
{
	velocity_ = VAdd(velocity_, vel);
}

void BaseEnemy::SetGetEffectPos(const VECTOR& pos)
{
	get_effect_->SetPos(pos);
}

void BaseEnemy::SetGotEffectPos(const VECTOR& pos)
{
	got_effect_->SetPos(pos);
}

void BaseEnemy::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

void BaseEnemy::SetPosIsGot(const VECTOR& pos)
{
	//collisionの位置更新も行うsetposとなります
	pos_ = pos;
	collision_data_.pos = pos;
	pos_.y -= collision_data_.r;
}