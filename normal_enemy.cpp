
#define _USE_MATH_DEFINES
#include <math.h>
#include<map>
#include <random>

#include"normal_enemy.h"
#include"situation.h"
#include"rot_function.h"
#include"Lerp.h"
#include"vector_assistant.h"
#include"collision_base.h"
#include"collision_sphere.h"
#include"stage.h"

NormalEnemy::NormalEnemy(const TCHAR* model_path, const VECTOR& pos, const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect, float speed, float fleeping_speed, AlertState alert, float fov, std::shared_ptr<Stage> stage)
	:EnemyBase(MV1LoadModel(model_path),pos,scale,dir,get_effect,got_effect,speed,fleeping_speed,alert,fov,stage, std::make_shared<CollisionSphere>(VGet(pos.x, (pos.y + 3.f), pos.z), 3.f),1.5f)
{
	const float kCollRadius = 3.f;
	const float kGravityCollRadius = kCollRadius - 0.1f;
	const VECTOR kGravityCollPos	= VGet(pos.x, (pos.y + kGravityCollRadius), pos.z);
	collision_data_.name = CollisionName::kSphere;
	collision_data_.pos = pos;
	collision_data_.r = 3.f;
	collision_data_.ver = 0.0;
	total_vel_ = VGet(0, 0, 0);
	//反転するときの値
	lerp_timer_ = 0.f;
	target_rot_ = 0.f;
	is_return_ = FALSE;
	lerp_flag_ = FALSE;

	fall_speed_ = 0.f;

	gravity_check_coll_ = std::make_shared<CollisionSphere>(kGravityCollPos, kGravityCollRadius);
	wait_timer_ = new ConditionTimer(kWaitTime);
}

NormalEnemy::~NormalEnemy()
{

}

//private

void NormalEnemy::DecideNextPos()
{

	// ここで次行く場所の指定を行う

	// 今いるwaypointの知り合いを受け取る
	auto way_points = GetNeighbors();
	int neighbors_num = 0;

	std::map<int, std::shared_ptr<WayPoint>> neighbors;
	//知り合いの中でランダムでえらぶ

	for (auto way_point : way_points)
	{
		neighbors[neighbors_num] = way_point;
		neighbors_num++;
	}
	neighbors_num--;
	//mapに代入した,代入した後にランダムで次のway_pointを選ぶ

	std::random_device rd;  // 非決定的乱数の種
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタ
	std::uniform_int_distribution<> rand(0, neighbors_num);	//人数分ランダム

	int num = rand(gen);

	//ランダム生成した物を入れる
	before_way_point_ = my_way_point_;
	my_way_point_ = neighbors[num];
	
	target_pos_ = my_way_point_->GetPos();
	lerp_flag_ = TRUE;

	wait_timer_->Reset();

}

bool NormalEnemy::CheckIsGound()
{
	return !stage_->CheckDownColl(gravity_check_coll_);
}

void NormalEnemy::Gravity()
{
	if (!is_ground_)
	{
		fall_speed_ -= kGravity * FPS::GetInstance().GetDeltaTime();
		velocity_ = VAdd(velocity_, VGet(0.f, fall_speed_, 0.f));
	}
	else
	{
		fall_speed_ = 0.f;
	}
}

AnimationType NormalEnemy::ChageAnimType(AnimationType now, AnimationType next)
{
	return (now == next) ? now : next;
}

//public

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

	rot_.y = VectorAssistant::GetPlaneRot(dir_);

	lerp_timer_ = 0.f;
	lerp_flag_ = TRUE;
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

void NormalEnemy::StanInit(std::shared_ptr<Player> player)
{
	// 攻撃を受けた時のInit
	// スタンの時間をリセットする
	stan_timer_->Reset();
	// アニメーションの適応をする
	now_anim_type_ = AnimationType::kStan;
}

void NormalEnemy::AlertInit(std::shared_ptr<Player> player)
{
	//タイマーのリセット
	alert_timer_->Reset();
	is_alert_ = TRUE;
	//独自のアニメーションも再生させたい(探しているような)
	now_anim_type_ = AnimationType::kAlert;
}


void NormalEnemy::FleepingInit(std::shared_ptr<Player> player)
{
	//ここでrotを指定してdirも指定する。
	//とりあえず反転して逃げさせる。

	//playerとenemyのposで逃げるのを指定

	DecideFirstFleepingPlace(player);

	//waypointが決まったのでway_pointに向かわせる

	is_fleeping_ = TRUE;
	lerp_flag_ = TRUE;
	now_anim_type_ = AnimationType::kFastRun;

	dir_ = VectorAssistant::GetDir(target_pos_, pos_);
	rot_.y = VectorAssistant::GetPlaneRot(dir_);
}


void NormalEnemy::AddAnim()
{
	//アニメーションスピード
	const float kAnimationWalkSpeed			= 3.f;
	const float kAnimationSurpriseSpeed	= 10.f;
	const float kAnimationFastRunSpeed	= 5.f;
	const float kAnimationIdleSpeed			= 4.f;
	const float kAnimationAlertSpeed			= 1.f;
	const float kAnimationStanSpeed			= 2.f;

	const char* kAnimationIdlePath			= "data/model/character/enemy/animation/Standing_W_Briefcase_Idle.mv1";
	const char* kAnimationWalkPath			= "data/model/character/enemy/animation/Walking.mv1";
	const char* kAnimationSurprisePath		= "data/model/character/enemy/animation/Joyful_Jump.mv1";
	const char* kAnimationFastRunPath		= "data/model/character/enemy/animation/Standard_Run.mv1";
	const char* kAnimationAlertPath			= "data/model/character/enemy/animation/Standing_Cover_Turn.mv1";
	const char* kAnimationStanPath			= "data/model/character/enemy/animation/Female_Dynamic_Pose.mv1";

	//各アニメーションを生成する
	AnimationData idle;
	AnimationData walk;
	AnimationData surprise;
	AnimationData fast_run;
	AnimationData alert;
	AnimationData stan;
	
	//アニメーションをロード
	Load(idle,			kAnimationIdlePath,			AnimationType::kIdle,			model_, 1, kAnimationIdleSpeed			);
	Load(walk,			kAnimationWalkPath,			AnimationType::kWalk,		model_, 1, kAnimationWalkSpeed		);
	Load(surprise,	kAnimationSurprisePath,		AnimationType::kSurprise,	model_, 1, kAnimationSurpriseSpeed	);
	Load(fast_run,	kAnimationFastRunPath,		AnimationType::kFastRun,	model_, 1, kAnimationFastRunSpeed	);
	Load(alert,			kAnimationAlertPath,			AnimationType::kAlert,		model_, 1, kAnimationAlertSpeed		);
	Load(stan,			kAnimationStanPath,			AnimationType::kStan,		model_, 1, kAnimationStanSpeed		);
	
	animation_->Add(idle);
	animation_->Add(walk);
	animation_->Add(surprise);
	animation_->Add(fast_run);
	animation_->Add(alert);
	animation_->Add(stan);
}


void NormalEnemy::Update(std::shared_ptr<Player> player, bool& got)
{

	velocity_ = VGet(0, 0, 0);
	
	// 着地判定
	is_ground_ = CheckIsGound();
	
	// すでにゲットもしくは、hitしているならこの関数は回さない
	if (!is_get_)
	{
		// ここでまだ捕まっていないときは
		player->IsHitEnemy(this, got);
	}
	else
	{
		return;
	}

	// 状態変化
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
	

	//stateによるupdate
	state_->Update(this, player);

	Gravity();

	//アニメーションの更新
	AnimationUpdate();

	velocity_ = stage_->CheckCollision(coll_, velocity_);
	//ポジションの更新

	pos_ = VAdd(pos_, velocity_);
	coll_->Update(velocity_);
	gravity_check_coll_->Update(velocity_);
	//当たり判定の位置は半径分上げる
	collision_data_.pos = pos_;
	collision_data_.pos.y += collision_data_.r;
	
}


void NormalEnemy::Patrolling()
{
	VECTOR vel = VGet(0,0,0);

	//線形保管でよくね
	//velにlerpのやつを代入
	if (lerp_flag_)
	{
		now_anim_type_ = ChageAnimType(now_anim_type_, AnimationType::kWalk);
		vel = NormalLerp(pos_, target_pos_,(speed_ * delta_time_),lerp_flag_);
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
	total_vel_ = VAdd(total_vel_,vel);

	rot_.y = atan2f(-dir_.x , -dir_.z);
	
	// ここでway_pointのcheckを行う
	// my_way_pointから知っているneighborsに向かわせる
	// my_way_point付近にいるのを感知する関数を用意

	

}


void NormalEnemy::Surprise()
{

}

void NormalEnemy::Stan()
{
	// タイマーをupdateさせます
	stan_timer_->Update();
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
	VECTOR vel = VGet(0.f, 0.f, 0.f);

	if (TRUE)
	{

		if (lerp_flag_)
		{
			vel = NormalLerp(pos_, target_pos_, fleeping_speed_, lerp_flag_);
		}
		else
		{
			DecideFleepingPlace(player, my_way_point_);
		}
		
		// ラープし終わったら新しい目標地点を選ぶ
		



		velocity_ = VAdd(velocity_, VScale(vel, delta_time_));
	}
	else
	{
		//定数
		const float kFleepingMax = 30.f;

		//ここで逃げる

		// どう逃げさせようかな
		// 一定距離うごいたら初期化させexitさせていいと思う

		

		vel = VScale(dir_, fleeping_speed_);

		velocity_ = VAdd(velocity_, VScale(vel, delta_time_));

		total_vel_ = VAdd(total_vel_, velocity_);

		//ここでtotal_vel_がまだ逃げ切ってないときは
		if (VSize(total_vel_) > kFleepingMax)
		{
			total_vel_ = VGet(0, 0, 0);
			is_fleeping_ = FALSE;
		}
	}
}







//洗濯ものです