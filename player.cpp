#include<iostream>
#include<vector>
#include<string>
#define _USE_MATH_DEFINES
#include <math.h>

//#include"mixamo_fram.h"
#include"player.h"
#include"camera.h"
#include"brain.h"
#include"keyconfig.h"
#include"weapon_base.h"
#include"input.h"
#include"stage.h"
#include"bat.h"
#include"warp_rod.h"
#include"wizard_staff.h"
#include"collision.h"
#include"situation.h"
#include"debug.h"
#include"weapon_checker.h"
#include"character_base.h"
#include"collision_base.h"
#include"collision_capsule.h"


Player::Player(VECTOR pos, int pad_num,int div, float r, float vertical_num)
	: model_(MV1LoadModel(kModelPath))
	, pad_input_num_(pad_num)
	, weapon_(nullptr)
	, now_type_(AnimationType::kNothing)
	, target_rot_(0.0f)
	, before_rot_(0.0f)
	, now_state_(PlayerState::kStand)
{
	capsule_.r = r;
	capsule_.div_num = div;
	capsule_.vertical_num = vertical_num;
	is_move_ = FALSE;
	is_camera_blend_ = FALSE;
	is_attack_ = FALSE;
	is_super_attack_ = FALSE;
	is_switch_weapon_ = FALSE;
	animation_ = std::make_shared<Animation>();
	Init(pos);
	super_attack_ = new SuperAttack(VGet(0, 0, 0), "");

	
	coll_ = std::make_shared<CollisionCapsule>(VAdd(pos, VGet(0.f, r, 0.f)), VAdd(pos, VGet(0.f, vertical_num, 0.f)), r);

	MATRIX pos_matrix = MGetTranslate(pos_);

	MATRIX rotation_matrix = MGetRotY(rotation_.y);

	model_matrix_ = MMult(MMult(
		MGetRotY(rotation_.y), MGetScale(VGet(0.01f, 0.01f, 0.01f))), pos_matrix);


	MV1SetMatrix(model_, model_matrix_);

}

Player::~Player()
{
	delete weapon_;
	delete super_attack_;
	delete super_weapon_spin_effect_;
	delete sound_vibration_;
}

/*--------------------private--------------------------*/


void Player::MakeTargetRot(const VECTOR& target_pos, float& target_rot)
{
	
	//新しいVECTORを作る(rotation)
	VECTOR rot_vec = VGet(target_pos.x - pos_.x,0.0f,target_pos.z - pos_.z);

	//タンジェントの解を求める
	float tan_num = 0;

	if (rot_vec.x == 0.0f)
	{
		if (rot_vec.z > 0.0f)
		{
			target_rot = static_cast<float>((M_PI / 180) * 90);
		}
		else
		{
			target_rot = -1 * (static_cast<float>((M_PI / 180) * 90));
		}
		
	}
	else
	{
		tan_num = rot_vec.z / rot_vec.x;
		target_rot = atanf(tan_num);
	}
	
	
	///printfDx("%f", target_rot);


}


bool Player::SuperAttackCondition()
{

	if (!(super_attack_->GetIsReady()))
	{
		return FALSE;
	}

	if (!is_ground_)
	{
		return FALSE;
	}

	if (!(now_weapon_name_ == WeaponName::kBugNet))
	{
		return FALSE;
	}

	if (now_type_ >= AnimationType::kAttack)
	{
		return FALSE;
	}

	if (is_super_attack_)
	{
		return FALSE;
	}



	if (!(Input::GetInstance().CheckInputPadButton(PadConfig::kSuperAttackButton) == InputState::kPush ||
		Input::GetInstance().CheckInputMouse(KeyConfig::kSuperAttackKey) == InputState::kPush))
	{
		return FALSE;
	}

	return TRUE;
}

/*------------------------public---------------------------*/

void Player::Init(VECTOR pos)
{
	
	
	now_type_ = AnimationType::kIdle;

	//animation_->Attach(now_type_);

	before_type_ = AnimationType::kNothing;
	before_before_type_ = AnimationType::kNothing;

	//int
	frame_num_ = 0;

	//float
	pos_ = pos;
	fall_speed_ = 0.0f;
	velocity_ = VGet(0, 0, 0);
	direction_ = VGet(0, 0, 0);
	rotation_ = VGet(0, 0, 0);
	before_rot_ = 0.0f;


	//bool
	is_ground_ = TRUE;
	is_target_ = FALSE;
	
	now_weapon_name_ = WeaponName::kBat;
	AddAnim();
	AttachWeapon(now_weapon_name_);

}


void Player::Draw()
{

	//キャラクター表示
	//MV1SetDifColorScale(model_, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	MV1DrawModel(model_);
	super_attack_->Draw();
	if (weapon_ != nullptr)
	{
		weapon_->Draw(delta_time_);
		//Situation::GetInstance().SetGetSituationPos(weapon_->GetCollisionData().pos);
	}

	

}


void Player::Debug()
{
	if (Debug::GetInstance().GetDisp())
	{
		
		coll_->Debug();

		//
		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), GetColor(0, 0, 0), "---------player--------");
		Debug::GetInstance().Add();

		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), GetColor(0, 0, 0), "pos");
		Debug::GetInstance().Add();
		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), GetColor(0,0,0), "x : %.2f,y : %.2f,z : %.2f", pos_.x, pos_.y, pos_.z);
		Debug::GetInstance().Add();

		DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), GetColor(0, 0, 0), "%.2f", sound_vibration_->GetNum());
		Debug::GetInstance().Add();

		super_attack_->Debug();

	}
}


void Player::AddAnim()
{

	/*--キャラクターのダウンロード--*/

	

	/*-----ダウンロードするアニメーション----*/


	AnimationData idle;
	AnimationData walk;
	AnimationData slow_run;
	AnimationData fast_run;
	AnimationData jumping_up;
	AnimationData jumping_down;
	AnimationData sword_slash_attack;
	AnimationData super_attack_first;

	



	char idle_path[256] = "data/animation/Idle.mv1";
	char walk_path[256] = "data/animation/Walking.mv1";
	char slow_run_path[256] = "data/animation/Slow_Run.mv1";
	char fast_run_path[256] = "data/animation/Fast_Run.mv1";
	char jumping_up_path[256] = "data/animation/Jumping_Up.mv1";
	char jumping_down_path[256] = "data/animation/Jumping_Down.mv1";
	char sword_slash_path[256] = "data/animation/SwordSlash.mv1";
	char super_attack_path[256] = "data/animation/Standing_2H_Cast_Spell_01.mv1";

	//アニメーションのロード

	Load(idle, idle_path,
		AnimationType::kIdle, model_, 0, 3.0f);

	Load(walk, walk_path,
		AnimationType::kWalk, model_, 0, 3.0f);

	Load(slow_run, slow_run_path,
		AnimationType::kSlowRun, model_, 0, 3.0f);

	Load(fast_run, fast_run_path,
		AnimationType::kFastRun, model_, 0, 3.0f);

	Load(jumping_up, jumping_up_path,
		AnimationType::kJumpUp, model_, 0, 2.0f);

	Load(jumping_down, jumping_down_path,
		AnimationType::kJumpDown, model_, 0, 2.0f);

	Load(sword_slash_attack, sword_slash_path,
		AnimationType::kSwordSlash, model_, 0, 4.0f);

	Load(super_attack_first, super_attack_path,
		AnimationType::kSuperAttackFirst, model_, 0, 3.0f);

	//アニメーションを追加
	animation_->Add(idle);
	animation_->Add(walk);
	animation_->Add(slow_run);
	animation_->Add(fast_run);
	animation_->Add(jumping_up);
	animation_->Add(jumping_down);
	animation_->Add(sword_slash_attack);
	animation_->Add(super_attack_first);
}

void Player::SetDeltaTime(const float& delta_time)
{

	//ゲット時はデルタタイムをゼロにする
	if (Situation::GetInstance().GetSituationName() == SituationName::kGet)
	{
		delta_time_ = 0.0f;
		animation_->SetDeltaTime(delta_time_);
		super_attack_->SetDeltaTime(delta_time_);
		weapon_->SetDeltaTime(delta_time_);
	}
	else
	{
		delta_time_ = delta_time;
		animation_->SetDeltaTime(delta_time);
		super_attack_->SetDeltaTime(delta_time_);
		weapon_->SetDeltaTime(delta_time_);
		
	}
	super_weapon_spin_effect_->SetDeltaTime(delta_time);
	
}




void Player::AttachWeapon(WeaponName name)
{
	if (weapon_ != nullptr)
	{
		weapon_ = nullptr;
	}
	


	MATRIX pos_matrix = MGetTranslate(pos_);
	MATRIX rotation_matrix = MGetRotY(rotation_.y);

	model_matrix_ = MMult(MMult(
		MGetRotY(rotation_.y), MGetScale(VGet(0.01f, 0.01f, 0.01f))), pos_matrix);

	MV1SetMatrix(model_, model_matrix_);
	
	frame_num_ = MV1SearchFrame(model_, "mixamorig:RightHand");

	MATRIX frame_mat = 
		MV1GetFrameLocalWorldMatrix(model_, frame_num_);


	

	//名前によってかえる

	switch (name)
	{

	case WeaponName::kBat:

		weapon_ = new Bat();

		break;

	case WeaponName::kBugNet:

		weapon_ = new WarpRod();

		break;

	case WeaponName::kWizardStaff:

		weapon_ = new WizardStaff();

		break;
	}

	now_weapon_name_ = name;
	WeaponChecker::GetInstance().SetWeaponName(now_weapon_name_);


	if (weapon_ != nullptr)
	{
		weapon_->SetModelMatrix(frame_mat);
		weapon_->SetPos(MV1GetFramePosition(model_, frame_num_));
	}
	
}


void Player::Update(Stage& stage,float target_rot)
{
	VECTOR camera_pos = Camera::GetInstance().GetPos();
	//サウンドのリセット
	sound_vibration_->Reset();

	//AttachWeapon(frame_path_->RIGHT_HAND);
	// ターゲットを切り替えた時のrotationを色んな奴に持たすわけにはいかないのでplayerに持たせる、
	// updateにはposだけにしといていいと思う(引き数)

	super_attack_->Update();

	InputMovement(camera_pos, target_rot);

	if (VSize(velocity_) != 0.f)
	{
		direction_ = VNorm(velocity_);
	}
	
	if (is_super_attack_)
	{
		super_attack_->SetPos(VAdd(pos_, VGet(0, 150, 0)),pos_);
		super_attack_->EffectUpdate();
	}

	if (AnimationType::kAttack > now_type_  && !is_super_attack_)
	{
		VECTOR before_pos = pos_;
		pos_ = stage.CheckCollision(*this, coll_,velocity_);

		velocity_ = VSub(pos_, before_pos);

		//pos_ = VAdd(pos_, velocity_);
		//当たり判定の更新
		coll_->Update(velocity_);
		capsule_.start_pos = pos_;
		capsule_.start_pos.y += capsule_.r;
		capsule_.end_pos = capsule_.start_pos;
		capsule_.end_pos.y += capsule_.vertical_num;
	}

	//ここで位置の更新もしておく
	//ここでのsetをやめる(ゲット時)
	
	if (Situation::GetInstance().GetSituationName() != SituationName::kGet)
	{
		MATRIX pos_matrix = MGetTranslate(pos_);
		MATRIX rotation_matrix = MGetRotY(rotation_.y);

		model_matrix_ = MMult(MMult(
			MGetRotY(rotation_.y), MGetScale(VGet(0.01f, 0.01f, 0.01f))), pos_matrix);

		MV1SetMatrix(model_, model_matrix_);

		if (weapon_ != nullptr)
		{
			//あれの時なんかおかしいです
			//ひっさつわざのweaponに切り替えた時
			auto test = GetFrameMatrix();
			//matの更新やめません
			weapon_->SetModelMatrix(test);
			weapon_->SetPos(MV1GetFramePosition(model_, frame_num_));
		}

	}

	

}

void Player::InputMovement(const VECTOR& pos,float& rotation)
{
	
	VECTOR velocity = { 0.0f,0.0f,0.0f };

	float speed = 0.0f;

	is_move_ = FALSE;

	//direction_ = VGet(0, 0, 0);

	/*(PadConfig::kLeftButton)*/

	

	CheckDirection(pos, rotation);

	if (!is_super_attack_ && now_type_ < AnimationType::kAttack)
	{
		if (is_move_)
		{
			/*左スティックの入力量をみる*/
			if (Input::GetInstance().CheckInputKey(KeyConfig::kDashKey) > InputState::kOff || Input::GetInstance().GetPadStickVertical(StickType::kLeft) > 200)
			{
				speed = kDashSpeed;

				now_type_ = AnimationType::kFastRun;
				now_state_ = PlayerState::kRun;
				sound_vibration_->Add(kFastRunSound);
			}
			else if (Input::GetInstance().CheckInputKey(KeyConfig::kWalkKey) > InputState::kOff || (Input::GetInstance().GetPadStickVertical(StickType::kLeft) > 50 &&
				Input::GetInstance().GetPadStickVertical(StickType::kLeft) < 150))
			{
				speed = kWalkSpeed;

				now_type_ = AnimationType::kWalk;
				now_state_ = PlayerState::kWalk;

				sound_vibration_->Add(kWalkSound);
				
			}
			else
			{
				speed = kNormalSpeed;

				now_type_ = AnimationType::kSlowRun;
				now_state_ = PlayerState::kSlowRun;

				sound_vibration_->Add(kNormalRunSound);
			}

		}
		else
		{
			
		}
		

	}
	
	velocity = VScale(direction_, speed);
	if (VSize(velocity) != 0.f)
	{
		direction_ = VNorm(velocity);
	}
	

	JumpAction(velocity);


	if (is_super_attack_)
	{
		
		velocity_ = VGet(0.f, 0.f, 0.f);
	}

	velocity_ = VScale(velocity, delta_time_);
	

	if (!is_ground_)
	{
		

		if (velocity_.y > 0.0f)
		{
			now_type_ = AnimationType::kJumpUp;
			now_state_ = PlayerState::kJump;
		}
		else if (velocity_.y < -0.1f)
		{
			now_type_ = AnimationType::kJumpDown;
			now_state_ = PlayerState::kFall;
		}
		else
		{
			now_type_ = before_type_;
		}


		
	}



	//必殺技(カメラが動いてない)
	//攻撃

	

	

	
	//棒を振る系のやつ
	if ((Input::GetInstance().CheckInputMouse(KeyConfig::kAttackKey) == InputState::kPush) && !(is_super_attack_))
	{
		if (is_ground_ && weapon_->GetName() != WeaponName::kWizardStaff)
		{
			now_type_ = AnimationType::kSwordSlash;
			now_state_ = PlayerState::kAttack;
		}
	}

	//必殺技による武器替え
	if (SuperAttackCondition())
	{
		is_super_attack_ = TRUE;
		now_type_ = AnimationType::kSuperAttackFirst;
		super_attack_->Init();
	}
	
	if (VSize(velocity) != 0)
	{
		direction_ = VNorm(velocity);
	}
	else
	{
		if (now_type_ < AnimationType::kAttack)
		{
			now_type_ = AnimationType::kIdle;
			now_state_ = PlayerState::kStand;
		}
		
	}
	

	if (before_type_ > AnimationType::kAttack)
	{
		if (animation_->IsPlay())
		{
			now_type_ = before_type_;
		}
		else
		{
			is_attack_ = FALSE;
			now_type_ = AnimationType::kIdle;
		}
	}

	

	//武器切り替えのやーつ
	if (!(now_type_ > AnimationType::kAttack) && !is_super_attack_)
	{
		WeaponName next_name = WeaponName::kNothing;

		if (Input::GetInstance().CheckInputPadButton(PadConfig::kSwitchWarpRodButton) == InputState::kPush ||
			Input::GetInstance().CheckInputKey(KeyConfig::kSwicthBatKey) == InputState::kPush)
		{
			next_name = WeaponName::kBugNet;
		}

		if (Input::GetInstance().CheckInputPadButton(PadConfig::kSwitchBatButton) == InputState::kPush ||
			Input::GetInstance().CheckInputKey(KeyConfig::kSwicthWarpRodKey) == InputState::kPush)
		{
			next_name = WeaponName::kBat;
		}
		


		if (now_weapon_name_ != next_name && next_name != WeaponName::kNothing)
		{
			AttachWeapon(next_name);
		}

	}

	


	/*----------------武器が吸引機のときはカメラを適応させる---------------*/

	if ((Situation::GetInstance().GetSituationName() != SituationName::kGet))
	{
		if (now_weapon_name_ >= WeaponName::kWizardStaff)
		{
			Situation::GetInstance().SetSituation(SituationName::kVacuum);
		}
		else
		{
			Situation::GetInstance().SetSituation(SituationName::kNothing);
			is_vacuum_ = FALSE;
		}
	}

	

	
	if (before_type_ != now_type_)
	{
		
		if (!(animation_->GetBlendFlag()))
		{
			if (!(before_type_ == AnimationType::kNothing))
			{
				animation_->InitBlend(now_type_, before_type_);
			}

			animation_->Attach(now_type_);

			before_before_type_ = before_type_;
			before_type_ = now_type_;

			animation_->SetBlend(TRUE);

		}
		else
		{
			if (now_type_ == AnimationType::kSuperAttackFirst)
			{
				animation_->Detach(before_type_);
				animation_->Attach(now_type_);
				before_before_type_ = before_type_;
				before_type_ = now_type_;

			}
		}
	}

	animation_->Update(now_type_);
	if (animation_->GetBlendFlag())
	{
		animation_->Update(before_type_);
	}

	
}


void Player::CheckDirection(const VECTOR& pos, float& rotation)
{
	
	float constant = 0.0f;

	MakeLine(constant, pos);

	VECTOR direction = VGet(0, 0, 0);



	//どちらが前かの判別
	//原点からの距離を見る

	float my_scale = sqrt((pos_.x * pos_.x) + (pos_.z * pos_.z));
	float other_scale = sqrt((pos.x * pos.x) + (pos.z * pos.z));

	//キーを何個入力したか
	int input_count = 0;

	//回転量
	float rot = 0.0f;

	bool flag = FALSE;

	//振っているかの判断
	static bool rejected = FALSE;

	if (pos_.x > pos.x)
	{
		direction = VGet(1, 0, 0);	
	}
	else
	{
		direction = VGet(-1, 0, 0);
	}


	/*--------プレイヤーの操作--------*/

	if (!is_super_attack_ && !is_attack_)
	{
		//前
		if (Input::GetInstance().CheckInputKey(KeyConfig::kUpKey))
		{
			direction_ = VAdd(direction_, VGet(direction.x, 0, direction.x * constant));
			//rotation_ = VGet(0, rotation, 0);

			/*---例外処理(行列使ったらこんなことしなくて済んだかも)---*/

			if (!(Input::GetInstance().CheckInputKey(KeyConfig::kDownKey) > InputState::kOff))
			{
				rot += (static_cast<float>((M_PI / 180) * 0));

				input_count++;
			}
			is_move_ = TRUE;
		}

		//後ろ
		if ((Input::GetInstance().CheckInputKey(KeyConfig::kDownKey) > InputState::kOff))
		{
			direction_ = VAdd(direction_, VGet(-1.0f * (direction.x), 0, -1.0f * (direction.x * constant)));

			/*---例外処理---*/
			if (!(Input::GetInstance().CheckInputKey(KeyConfig::kUpKey) > InputState::kOff))
			{

				if ((Input::GetInstance().CheckInputKey(KeyConfig::kRightKey) > InputState::kOff))
				{
					rot += (static_cast<float>((M_PI / 180) * 180));
				}
				else if ((Input::GetInstance().CheckInputKey(KeyConfig::kLeftKey) > InputState::kOff))
				{
					rot += -1 * (static_cast<float>((M_PI / 180) * 180));
				}
				else
				{
					rot += (static_cast<float>((M_PI / 180) * 180));
				}

				input_count++;

			}
			is_move_ = TRUE;
		}


		//右
		if ((Input::GetInstance().CheckInputKey(KeyConfig::kRightKey) > InputState::kOff))
		{
			direction_ = VAdd(direction_, VGet(direction.x * constant, 0, -direction.x));

			/*---例外処理---*/
			if (!((Input::GetInstance().CheckInputKey(KeyConfig::kLeftKey) > InputState::kOff)))
			{
				rot += (static_cast<float>((M_PI / 180) * 90));
				input_count++;
			}
			//rotation_ = VAdd(rotation_,VGet(0, rotation + static_cast<float>((M_PI / 180) * 90), 0));
			is_move_ = TRUE;
		}

		//左
		if ((Input::GetInstance().CheckInputKey(KeyConfig::kLeftKey) > InputState::kOff))
		{
			direction_ = VAdd(direction_, VGet(-(direction.x * constant), 0, direction.x));

			/*---例外処理---*/
			if (!((Input::GetInstance().CheckInputKey(KeyConfig::kRightKey) > InputState::kOff)))
			{
				rot += -1 * static_cast<float>((M_PI / 180) * 90);

				input_count++;
			}
			is_move_ = TRUE;
			//rotation_ = VAdd(rotation_, VGet(0, rotation - static_cast<float>((M_PI / 180) * 90), 0));
		}

		/*---pad除外----*/

		if (Input::GetInstance().GetPadStickVertical(StickType::kLeft) > 50.0f)
		{
			rot += Input::GetInstance().GetPadStickRad(StickType::kLeft);

			direction_ = VAdd(direction_,
				VGet(-sinf(rot + rotation), 0.0f, -cosf(rot + rotation)));

			input_count++;
			is_move_ = TRUE;
		}

		//回転からdirectionを出すことができる
		//direction_.x = cosf(rot) * 1.0f;
		//direction_.z = sinf(rot) * 1.0f;

		//正規化
		if (VSquareSize(direction_) > 0)
		{
			direction_ = VNorm(direction_);
		}

	}

	/*---棒を振る--*/

	//とりあえず右スティックの入力量を受け取る
	//今連続でふれるようになってしまっている
	if (Input::GetInstance().GetPadStickVertical(StickType::kRight) > 150.f && now_weapon_name_ != WeaponName::kWizardStaff)
	{
		//元から振っているときはダメにする
		if (is_ground_ && !is_attack_ && !rejected)
		{
			
			target_rot_ = rotation + Input::GetInstance().GetPadStickRad(StickType::kRight);
			now_type_ = AnimationType::kSwordSlash;
			now_state_ = PlayerState::kAttack;
			is_attack_ = TRUE;
			if (target_rot_ > (static_cast<float>((M_PI / 180) * 180)))
			{
				target_rot_ = target_rot_ - (static_cast<float>((M_PI / 180) * 360));
			}

			if (target_rot_ < -(static_cast<float>((M_PI / 180) * 180)))
			{
				target_rot_ = target_rot_ + (static_cast<float>((M_PI / 180) * 360));
			}

			before_rot_ = target_rot_;
			rejected = TRUE;
		}
		
	}
	else
	{
		rejected = FALSE;
	}
	
	//
	SetLightDirection(VGet(direction.x, 0, direction.x * constant));

	if (input_count != 0 && !is_attack_)
	{
		target_rot_ = (rotation + (rot / input_count));

		if (target_rot_ > (static_cast<float>((M_PI / 180) * 180)))
		{
			target_rot_ = target_rot_ - (static_cast<float>((M_PI / 180) * 360));
		}

		if (target_rot_ < -(static_cast<float>((M_PI / 180) * 180)))
		{
			target_rot_ = target_rot_ + (static_cast<float>((M_PI / 180) * 360));
		}

		before_rot_ = rot / input_count;
	}

	camera_offset_dir = VGet(direction.x * constant, 0, -direction.x);

	//printfDx("%f\n", rotation);
	CheckReverseRot(rotation_.y, target_rot_);
	
}


void Player::CheckReverseRot(float& now_rot, float target_rot)
{
	//同じときは早期リターン
	if (now_rot == target_rot) { return; }
	
	//ここで180の値を宣言
	float simple_reverse_num = (static_cast<float>(M_PI / 180) * 180);

	/*---------------------------新しい処理----------------------------*/

	// まずは今の座標から目標の座標までの距離を求める
	// その距離が180度を越えるような大きさだと例外の処理を進める

	// 今からからターゲットまでの回転の距離
	float rot_distance = 0.0f;

	//回転量
	float rot_num = (static_cast<float>((M_PI / 180) * 10.f)) * (delta_time_ * 10);

	// 同じときは先にはじくようにしているので大丈夫
	// どちらが小さいかを見て小さいほうから大きいほうを引く
	if (now_rot < target_rot)
	{
		rot_distance = now_rot - target_rot;
	}
	else
	{
		rot_distance = target_rot - now_rot;
	}

	// rot_distanceの絶対値が180より大きいなら
	if (fabs(rot_distance) > simple_reverse_num)
	{
		//現在の回転量がマイナスなら
		if (now_rot < static_cast<float>((M_PI / 180) * 0))
		{
			now_rot -= rot_num;

			//-180を超えるとき
			if (now_rot < -simple_reverse_num)
			{
				//-180からどんだけ超えているのかを確認
				float over_num = now_rot + simple_reverse_num;

				//超過したときの+の値を代入
				now_rot = simple_reverse_num + over_num;

				if (now_rot < target_rot)
				{
					now_rot = target_rot;
				}

			}

		}
		else  //+なら
		{
			now_rot += rot_num;

			//180を超えるとき
			if (now_rot > simple_reverse_num)
			{
				//180からどんだけ超えているかを確認
				float over_num = now_rot - simple_reverse_num;

				//超過したときの-の値を代入
				now_rot = -simple_reverse_num + over_num;

				if (now_rot > target_rot)
				{
					now_rot = target_rot;
				}

			}
		}
	}
	else  //普通の処理
	{
		if (now_rot < target_rot)
		{
			now_rot += rot_num;

			if (now_rot > target_rot)
			{
				now_rot = target_rot;
			}
		}
		else
		{
			now_rot -= rot_num;

			if (now_rot < target_rot)
			{
				now_rot = target_rot;
			}
		}
	}
	

}



void  Player::JumpAction(VECTOR& velocity)
{
	//重力
	//fall_speed_ -= (kGravity * delta_time_);

	//地面にいるかの判定
	//is_ground_ = CheckGround();


	if (is_ground_)
	{

		if (Input::GetInstance().CheckInputKey(KeyConfig::kJumpKey) == InputState::kPush ||
			Input::GetInstance().CheckInputPadButton(PadConfig::kJumpButton) == InputState::kPush)
		{
			//ジャンプの処理
			fall_speed_ = kJumpPower;
			is_ground_ = FALSE;
			now_type_ = AnimationType::kJumpUp;
		}
	}
	else
	{
		fall_speed_ -= (kGravity * delta_time_);
	}
	
	VECTOR fall_velocity = VGet(0, fall_speed_, 0);
	velocity = VAdd(velocity, fall_velocity);


}


bool Player::CheckGround()
{
	if (pos_.y <= 0.0f)
	{
		fall_speed_ = 0.0f;
		return TRUE;
		
	}
	else
	{
		return FALSE;
	}
}


void Player::OnHitRoof()
{
	velocity_.y = -velocity_.y;
}


void Player::OnHitFloor()
{
	velocity_.y = 0.0f;
}


void Player::TestFunc()
{
	frame_num_ = MV1GetFrameNum(model_);


	for (int i = 0; i < frame_num_; i++)
	{
		// フレーム名の描画
		DrawFormatString(0, i * 15, GetColor(255, 255, 255), "Name         %s", MV1GetFrameName(model_, i));
	}


}


void Player::MakeLine(float& constant, const VECTOR& pos)
{
	//直線のvector
	VECTOR  distance = VGet(pos.x - pos_.x, 0, pos.z - pos_.z);

	if (distance.x != 0.0f)
	{
		constant = distance.z / distance.x;
	}
	else
	{
		constant = distance.z;
	}

}


void Player::IsHitEnemy(EnemyBase* enemy, bool& got)
{
	
	//ここでweaponのアップデートをする

	if (weapon_->GetName() == WeaponName::kWizardStaff)
	{
		//回しているradの値を受け取る
		float stick_spin_rad = Input::GetInstance().GetStickSpin(StickType::kRight);


		if (stick_spin_rad != 0.f && (Input::GetInstance().GetPadStickVertical(StickType::kRight) > kPadSpinMin))
		{
			weapon_->Update(enemy,fabs(stick_spin_rad));
			now_state_ = PlayerState::kAttack;
			super_weapon_spin_effect_->SetPos(weapon_->GetPos());
			super_weapon_spin_effect_->Play();
			Vibration(kVacuumVibration);
			sound_vibration_->Add(kVacuumSound);
			is_vacuum_ = TRUE;
		}
		else
		{
			super_weapon_spin_effect_->End();
			if (Situation::GetInstance().GetSituationName() == SituationName::kVacuum)
			{
				Situation::GetInstance().SetSituation(SituationName::kNothing);
			}
			is_vacuum_ = FALSE;
		}
	}


	//他のものが攻撃にあたっている時は処理を回さない

	if (now_state_ == PlayerState::kAttack)
	{
		// 武器と敵の当たり判定をします
		if (SphereCapsuleCollision(weapon_->GetCollisionData(), enemy->GetCollisionData()))
		{
			
			
			//武器が違うときは違う結果にしたい

			switch (weapon_->GetName())
			{
				//batの時
			case WeaponName::kBat:

				printfDx("bat");
				printfDx("に当たっています\n");

				//ここでsituationを切り替える


				break;

				//ワープポイの時
			case WeaponName::kBugNet:
			case WeaponName::kWizardStaff:

				//printfDx("WarpRod");
				//printfDx("に当たっています\n");
				//位置の調整を行う。武器の位置に沿わす
				enemy->SetPosIsGot(weapon_->GetCollisionData().pos);
				
				//ここでsituationを切り替える(getにする)
				Situation::GetInstance().SetSituation(SituationName::kGet);
				Situation::GetInstance().SetGetSituationPos(enemy->GetCollisionData().pos);
				enemy->SetGetEffectPos(enemy->GetPos());
				enemy->SetGotEffectPos(enemy->GetPos());
				enemy->SetIsGet(TRUE);
				got = TRUE;

				//effectをセッティング
				enemy->SetGetEffectPos(enemy->GetPos());

				SetDeltaTime(0.f);
				enemy->SetDeltaTime(0.f);
				
				//もうここらへんでストップさせなきゃいけない1f遅れているのが何かおかしい


				break;


			}

			// 当たっているときにカメラの処理も一緒にしたい
			// posを取得しといて、次のアップデートの処理の時にはじめるのか、それともRateUpdateというものを作り、ゲットしていたら、その時の処理を行う専用のものを用意するのか


		}
	}
	
	//いまis_super_attackは変身になっている<-良くない。解釈が違う
	//ぷれいやーが必殺技中ならば処理を変えたい
	//武器が何かによって変えようかな
	if (weapon_->GetName() > WeaponName::kSuperAttack)
	{
		//printfDx("必殺weapon");
	}
	
}

void Player::Vibration(const VibrationData& data)
{
	// ほしいのは時間と、振動の強さ
	// パッドしんどう
	StartJoypadVibration(Input::GetInstance().GetPadNom(), data.power, data.time, -1);
}


MATRIX Player::GetFrameMatrix()
{
	return MV1GetFrameLocalWorldMatrix(model_,frame_num_);
}

VECTOR Player::GetWeaponPos()
{
	return weapon_->GetPos();
}

