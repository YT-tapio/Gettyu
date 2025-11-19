#include<math.h>
#include"base_enemy.h"
#include"situation.h"
#include"patrolling.h"
#include"debug.h"
#include"animation.h"
#include"const_rad.h"

BaseEnemy::BaseEnemy(const int model, const VECTOR& pos,
	const VECTOR& scale, const VECTOR& rot, Effect* get_effect,Effect* got_effect,float speed, float fleeping_speed, AlertState alert, float fov)
{
	fsm_		= std::make_shared<EnemyFSM>();
	navigation_ = std::make_shared<Navigation>();
	animation_	= std::make_shared<Animation>();


	now_anim_type_				= AnimationType::kNothing;
	before_anim_type_			= AnimationType::kNothing;
	before_before_anim_type_	= before_anim_type_;
	//モデルのダウンロード
	model_ = model;
	if (model_ == -1)
	{
		//printfDx("enemyのモデル読み込み失敗\n");
	}

	state_ = nullptr;

	//VECTOR
	pos_ = pos;
	dir_ = VGet(0.f, 0.f, 0.f);
	rot_ = rot;
	velocity_ = VGet(0.f, 0.f, 0.f);
	scale_ = scale;

	target_pos_ = VGet(0.f, 0.f, 0.f);
	start_pos_ = VGet(0.f, 0.f, 0.f);

	my_way_point_ = nullptr;
	before_way_point_ = nullptr;

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
	fleeping_speed_ = fleeping_speed;
	
	alert_state_ = alert;

	//警戒度によって異なる数値
	switch (alert_state_)
	{
	case AlertState::kHigh:

		alert_dist_			= kAlertHigh;

		break;

	case AlertState::kNormal:

		alert_dist_			= kAlertNormal;
	
		break;

	case AlertState::kLow:

		alert_dist_			= kAlertLow;

		break;

	}

	//engagementも比を作ってそれでやる
	engagement_dist_ = kEngagementNormal * alert_dist_ / kAlertNormal;

	//もともとのやつとの比を作る、その比をtimerに掛ける
	alert_timer_ = new ConditionTimer(kNormalAlertTime * (alert_dist_ / kAlertNormal));


	fov_ = fov;
	debug_color_ = GetColor(255, 255, 255);
}

BaseEnemy::~BaseEnemy()
{
	//delete get_effect_;
	//delete got_effect_;
}


VECTOR BaseEnemy::GetNearWayPointPos()
{
	auto way_points = navigation_->GetWayPoint();

	VECTOR most_near_pos = VGet(0, 0, 0);
	// 自分のポジションからwaypointの距離をみて、範囲外なら仲間に入れない

	for (auto& way_point : way_points)
	{
		VECTOR pos = way_point->GetPos();

		//1番目は代入させる
		if (VSize(most_near_pos) == 0)
		{
			most_near_pos = pos;
			my_way_point_ = way_point;
		}

		//距離を出す
		float dist = VSize(VSub(pos, pos_));
		float most_near_dist = VSize(VSub(most_near_pos, pos_));

		//一番近いものよりも近いときはposを更新
		if (dist < most_near_dist)
		{
			most_near_pos = pos;
			before_way_point_ = my_way_point_;
			my_way_point_ = way_point;
		}
		

	}

	near_way_point_pos_ = most_near_pos;
	return most_near_pos;
}


std::vector<VECTOR> BaseEnemy::GetWayPointNeighborsPos()
{
	auto neighbors = GetNeighbors();
	std::vector<VECTOR> way_point_pos;

	for (auto neighbor : neighbors)
	{
		way_point_pos.push_back(neighbor->GetPos());
	}

	return way_point_pos;
}

std::vector<std::shared_ptr<WayPoint>> BaseEnemy::GetNeighbors()
{
	return navigation_->GetNeighbors(my_way_point_);
}

void BaseEnemy::DecideFirstFleepingPlace(std::shared_ptr<Player> player)
{
	// playerのposから遠い場所を指定する
	
	VECTOR player_pos = player->GetPos();

	//waypointを代入

	std::vector<std::shared_ptr<WayPoint>> way_points;

	way_points.push_back(my_way_point_);
	way_points.push_back(before_way_point_);

	// beforeのwaypointを所持させておきたい
	before_way_point_ = my_way_point_;
	my_way_point_ = GetFarWayPoint(player_pos, way_points);

	target_pos_ = my_way_point_->GetPos();
	lerp_flag_ = TRUE;

	// 行きたい方向にplayerがいるなら違うとこに向かわせる(後でやる)

}

std::shared_ptr<WayPoint> BaseEnemy::GetFarWayPoint(const VECTOR& pos, std::vector<std::shared_ptr<WayPoint>> way_points)
{
	//beforeとmyのどっちが近いのかの判断

	std::shared_ptr<WayPoint> point = nullptr;
	float most_far_size = 0.f;


	for (auto& way_point : way_points)
	{
		if (way_point != nullptr)
		{
			//何もないとき
			if (point == nullptr)
			{
				point = way_point;
				most_far_size = VSize(VSub(way_point->GetPos(), pos));
			}
			else
			{
				// nullptrではない時
				// サイズの確認
				float size = VSize(VSub(way_point->GetPos(), pos));

				//farよりも大きいのなら
				if (size > most_far_size)
				{
					//そのway_pointを代入
					most_far_size = size;
					point = way_point;
				}

			}
		}
	}


	return point;
	
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


void BaseEnemy::AnimationUpdate()
{
	if (before_anim_type_ != now_anim_type_)
	{

		if (!(animation_->GetBlendFlag()))
		{
			
			if (before_anim_type_ != AnimationType::kNothing)
			{
				animation_->InitBlend(now_anim_type_, before_anim_type_);
			}

			animation_->Attach(now_anim_type_);

			before_before_anim_type_ = before_anim_type_;
			before_anim_type_ = now_anim_type_;

			animation_->SetBlend(TRUE);

		}
		else
		{
			if (now_anim_type_ == AnimationType::kSuperAttackFirst)
			{
				animation_->Detach(before_anim_type_);
				animation_->Attach(now_anim_type_);
				before_before_anim_type_ = before_anim_type_;
				before_anim_type_ = now_anim_type_;
			}
		}
	}

	animation_->Update(now_anim_type_);
	if (animation_->GetBlendFlag())
	{
		animation_->Update(before_anim_type_);
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
		int red = GetColor(255, 0, 0);
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

		
		DrawSphere3D(pos_, alert_dist_, 20, debug_color_, debug_color_, FALSE);		// 警戒距離を可視化
		DrawSphere3D(pos_, engagement_dist_, 20, red, red, FALSE);					// 接敵距離

		//正面を出す
		DrawLine3D(pos_, VAdd(pos_, VScale(dir_, 5.f)), GetColor(255, 255, 255));
		DrawFov();
		
		//way_pointと結びつける
		DrawLine3D(near_way_point_pos_, pos_, GetColor(255, 255, 255));

		//enemy名
		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "----------enemy%d----------", i);
		Debug::GetInstance().Add();
		
		//ポジションを表示
		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "pos");
		Debug::GetInstance().Add();

		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "x : %.2f,y : %.2f,z : %.2f", pos_.x, pos_.y, pos_.z);
		Debug::GetInstance().Add();

		//
		if (state_ != nullptr)
		{
			switch (state_->GetName())
			{
			case StateName::kPatrolling:
				DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "state : patlloring");
				break;

			case StateName::kAlert:
				DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "state : alert");
				break;

			case StateName::kSurprise:
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

		

		if (my_way_point_ != nullptr)
		{
			DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), debug_color_, "/*--------%dのneighbors--------*/", my_way_point_->GetNum());
			Debug::GetInstance().Add();
			auto neighbors = my_way_point_->GetFriend();

			for (auto num : neighbors)
			{
				VECTOR neighbors_pos = navigation_->GetWayPointPos(num);

				Debug::GetInstance().VectorDraw(neighbors_pos);
			}
		}

		


		
		//navigationの可視化
		navigation_->Debug();

	}


}

void BaseEnemy::SetColor(int color)
{
	debug_color_ = color;
}

void BaseEnemy::SetDeltaTime(float delta_time)
{
	delta_time_ = delta_time;
	animation_->SetDeltaTime(delta_time_);
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

VECTOR BaseEnemy::DecideNextPlace()
{
	
	float rot = 0.f;
	VECTOR dir = VGet(0, 0, 0);

	//近くのwaypointのposを獲得
	VECTOR near_way_point_pos = GetNearWayPointPos();
	
	dir = VNorm(VSub(near_way_point_pos, pos_));

	target_pos_ = near_way_point_pos;
	return dir;
}