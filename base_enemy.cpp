#include<math.h>
#include"base_enemy.h"
#include"situation.h"
#include"patrolling.h"
#include"debug.h"

BaseEnemy::BaseEnemy(const int model, const VECTOR& pos,
	const VECTOR& scale, const VECTOR& rot, Effect* get_effect,Effect* got_effect,float speed,float alert_dist, float fov)
{
	fsm_ = std::make_shared<EnemyFSM>();

	//モデルのダウンロード
	model_ = model;
	if (model_ == -1)
	{
		//printfDx("enemyのモデル読み込み失敗\n");
	}

	state_ = std::make_shared<EnemyPatrolling>();

	//VECTOR
	pos_ = pos;
	dir_ = VGet(0.f, 0.f, 0.f);
	rot_ = rot;
	velocity_ = VGet(0.f, 0.f, 0.f);
	scale_ = scale;

	//dirはrotから求まる

	dir_ = VGet(-sinf(rot.y), 0.f, -cosf(rot.y));
	mat_ = MMult(MMult(MGetRotY(0.0f), MGetScale(scale_)), 
		MGetTranslate(pos_));

	is_get_ = FALSE;
	is_fleeping_ = FALSE;
	delta_time_ = 0.0f;

	get_effect_ = get_effect;
	got_effect_ = got_effect;

	speed_ = speed;
	alert_dist_ = alert_dist;
	fov_ = fov;
	debug_color_ = GetColor(255, 255, 255);
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

	mat_ = MMult(MMult(MGetRotY(rot_.y), MGetScale(scale_)), MGetTranslate(pos_));

	
	

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

	

	

}


void BaseEnemy::DrawFov()
{
	const float angle_scale = 5.f;

	//2本線出る
	float angle1 = rot_.y + (fov_ * 0.5f);
	float angle2 = rot_.y - (fov_ * 0.5f);

	//角度が出せたのでdirを出す
	VECTOR dir1 = VGet(-sinf(angle1), 0.f,-cosf(angle1));
	VECTOR dir2 = VGet(-sinf(angle2), 0.f, -cosf(angle2));

	//そのdirにscaleをかけて今のposにたす
	VECTOR fov_pos1 = VAdd(pos_,VScale(dir1, angle_scale));
	VECTOR fov_pos2 = VAdd(pos_,VScale(dir2, angle_scale));

	//posが出たので線を引く
	DrawLine3D(pos_, fov_pos1, GetColor(255, 255, 255));
	DrawLine3D(pos_, fov_pos2, GetColor(255, 255, 255));
}

void BaseEnemy::Debug(int i)
{
	//でばっくのシングルトンから今までのデバックのログ数を受け取りその量を受け取る
	if (Debug::GetInstance().GetDisp())
	{
		//当たり判定を表示
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

		//正面を出す
		DrawLine3D(pos_, VAdd(pos_, VScale(dir_, 5.f)), GetColor(255, 255, 255));
		DrawFov();


		//enemy名
		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "----------enemy%d----------", i);
		Debug::GetInstance().Add();
		
		//ポジションを表示
		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "pos");
		Debug::GetInstance().Add();

		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "x : %.2f,y : %.2f,z : %.2f", pos_.x, pos_.y, pos_.z);
		Debug::GetInstance().Add();

		//
		switch (state_->GetName())
		{
		case StateName::kPatrolling:
			DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "state : patlloring");
			break;

		case StateName::kAlert:
			DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "state : alert");
			break;

		case StateName::kFleeping:
			DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "stat : fleeping");
			break;
		}
		Debug::GetInstance().Add();
		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "rot : % .2f", rot_.y);

		Debug::GetInstance().Add();
	}


}

void BaseEnemy::SetColor(int color)
{
	debug_color_ = color;
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