#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include"player.h"
#include"keyconfig.h"
#include"weapon.h"



Player::Player(VECTOR pos, int model,int pad_num)
	: model_(model)
	, pad_input_num_(pad_num)
	, weapon_(nullptr)
	, now_type_(AnimationType::kNothing)
	, target_rot_(0.0f)
	, before_rot_(0.0f)
{
	//model_ = model;
	//pad_input_num_ = pad_num;
	Init(pos);
}

Player::~Player()
{

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




/*------------------------public---------------------------*/

void Player::Init(VECTOR pos)
{
	
	
	now_type_ = AnimationType::kIdle;

	//animation_.Attach(now_type_);

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
}


void Player::Draw()
{
	//MV1SetPosition(model_, pos_);

	MATRIX pos_matrix = MGetTranslate(pos_);
	MATRIX rotation_matrix = MGetRotY(rotation_.y);

	model_matrix_ = MMult(MMult(
		MGetRotY(rotation_.y), MGetScale(VGet(0.01f, 0.01f, 0.01f))),pos_matrix);

	//MV1SetRotationXYZ(model_, rotation_);
	DrawSphere3D(VGet(pos_.x, pos_.y + 15, pos_.z), 0.5f, 5, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

	DrawFormatString(200, 200, GetColor(255, 255, 255), "%f", rotation_.y);

	MV1SetMatrix(model_, model_matrix_);
	//MV1SetRotationXYZ(model_, rotation_);

	MV1DrawModel(model_);

	if (weapon_ != nullptr)
	{
		weapon_->Draw();
	}
	

	//animation_.Draw(now_type_);
	


	//TestFunc();
}


void Player::AddAnim(const AnimationData& animation_data)
{
	//アニメーションを追加
	animation_.Add(animation_data);
}


void Player::InputState()
{
	GetHitKeyStateAll(key_input_);

	GetJoypadXInputState(pad_input_num_, &pad_input_);
}


void Player::AttachWeapon(const TCHAR* frame_path, int model,float scale)
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
	
	frame_num_ = MV1SearchFrame(model_, frame_path);

	MATRIX frame_mat = 
		MV1GetFrameLocalWorldMatrix(model_, frame_num_);

	weapon_ = new Weapon(
		frame_mat, model,scale, 
		MV1GetFramePosition(model_, frame_num_));

}


void Player::Update(const VECTOR& pos, const float& rotation)
{

	// ターゲットを切り替えた時のrotationを色んな奴に持たすわけにはいかないのでplayerに持たせる、
	// updateにはposだけにしといていいと思う(引き数)

	float target_rot = rotation;
	

	InputMovement(pos, target_rot);
	if (AnimationType::kAttack > now_type_)
	{
		pos_ = VAdd(pos_, velocity_);
	}
	

	

	//武器を持たない設定にしているときは処理を回さない
	if (weapon_ != nullptr)
	{
		auto test = GetFrameMatrix();

		weapon_->SetMatrix(test);
		weapon_->SetPos(MV1GetFramePosition(model_, frame_num_));
	}

	printfDx("%f\n", target_rot);

	//rotation_.y = target_rot;

}

void Player::InputMovement(const VECTOR& pos,float& rotation)
{
	VECTOR velocity = { 0.0f,0.0f,0.0f };

	float speed = 0.0f;

	direction_ = VGet(0, 0, 0);

	/*(PadConfig::kLeftButton)*/

	CheckDirection(pos, rotation);


	if (key_input_[KeyConfig::kDashKey])
	{
		speed = kDashSpeed;

		now_type_ = AnimationType::kFastRun;
	}
	else if (key_input_[KeyConfig::kWalkKey])
	{
		speed = kWalkSpeed;

		now_type_ = AnimationType::kWalk;
	}
	else
	{
		speed = kNormalSpeed;

		now_type_ = AnimationType::kSlowRun;
	}

	velocity = VScale(direction_, speed);

	JumpAction(velocity);


	

	if (VSize(velocity) != 0)
	{
		direction_ = VNorm(velocity);
	}
	else
	{
		now_type_ = AnimationType::kIdle;
	}

	if (!is_ground_)
	{
		if (velocity_.y > 0)
		{
			now_type_ = AnimationType::kJumpUp;
		}
		else if(velocity_.y < 0)
		{
			now_type_ = AnimationType::kJumpDown;
		}
		
	}


	//velocity_ = VScale(velocity, delta_time_);

	/*---デバッグ用---*/
	if (key_input_[KEY_INPUT_1])
	{
		now_type_ = AnimationType::kIdle;
	}

	if (key_input_[KEY_INPUT_2])
	{
		now_type_ = AnimationType::kWalk;
	}

	if (key_input_[KEY_INPUT_3])
	{
		now_type_ = AnimationType::kSlowRun;
	}

	if (key_input_[KEY_INPUT_Q])
	{
		now_type_ = AnimationType::kSwordSlash;
	}

	/*
	if (now_type_ != AnimationType::kIdle)
	{
		now_type_ = AnimationType::kIdle;
	}
	*/


	if (before_type_ != now_type_ && !(animation_.GetBlendFlag()))
	{

		if (!(before_type_ == AnimationType::kNothing))
		{
			animation_.InitBlend(now_type_, before_type_);
		}

		animation_.Attach(now_type_);

		before_before_type_ = before_type_;
		before_type_ = now_type_;

		animation_.SetBlend(TRUE);

	}

	animation_.Update(now_type_);
	if (animation_.GetBlendFlag())
	{
		animation_.Update(before_type_);
	}



	velocity_ = VScale(velocity,delta_time_);


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

	if (pos_.x > pos.x)
	{
		direction = VGet(1, 0, 0);	
	}
	else
	{
		direction = VGet(-1, 0, 0);
	}


	/*--------プレイヤーの操作--------*/

	//前
	if (key_input_[KeyConfig::kUpKey] || pad_input_.ThumbLY > PadConfig::kUpStick)
	{
		direction_ = VAdd(direction_, VGet(direction.x, 0, direction.x * constant));
		//rotation_ = VGet(0, rotation, 0);

		/*---例外処理(行列使ったらこんなことしなくて済んだかも)---*/

		if (!(key_input_[KeyConfig::kDownKey]))
		{
			rot += (static_cast<float>((M_PI / 180) * 0));

			input_count++;
		} 

	}

	//後ろ
	if (key_input_[KeyConfig::kDownKey] || pad_input_.ThumbLY < PadConfig::kDownStick)
	{
		direction_ = VAdd(direction_, VGet(-1.0f * (direction.x), 0, -1.0f * (direction.x * constant)));

		/*---例外処理---*/
		if (!key_input_[KeyConfig::kUpKey])
		{
			if (key_input_[KeyConfig::kRightKey] || pad_input_.ThumbLX < PadConfig::kRightStick)
			{
				rot += (static_cast<float>((M_PI / 180) * 180));
			}
			else if(key_input_[KeyConfig::kLeftKey] || pad_input_.ThumbLX > PadConfig::kLeftStick)
			{
				rot +=  -1 * (static_cast<float>((M_PI / 180) *  180));
			}
			else
			{
				//前回を参照する
				if (before_rot_ > static_cast<float>((M_PI / 180) * 0))
				{
					rot += (static_cast<float>((M_PI / 180) * 180));
				}
				else
				{
					rot += -1 * (static_cast<float>((M_PI / 180) * 180));
				}
			}



			input_count++;
		}

	}


	//右
	if (key_input_[KeyConfig::kRightKey] || pad_input_.ThumbLX < PadConfig::kRightStick)
	{
		direction_ = VAdd(direction_, VGet(direction.x * constant, 0, -direction.x));

		/*---例外処理---*/
		if (!key_input_[KeyConfig::kLeftKey])
		{
			rot += (static_cast<float>((M_PI / 180) * 90));
			input_count++;
		}
		//rotation_ = VAdd(rotation_,VGet(0, rotation + static_cast<float>((M_PI / 180) * 90), 0));
	}

	//左
	if (key_input_[KeyConfig::kLeftKey] || pad_input_.ThumbLX > PadConfig::kLeftStick)
	{
		direction_ = VAdd(direction_, VGet(-(direction.x * constant), 0, direction.x));

		/*---例外処理---*/
		if (!key_input_[KeyConfig::kRightKey])
		{
			rot += -1 * static_cast<float>((M_PI / 180) * 90);

			input_count++;
		}

		//rotation_ = VAdd(rotation_, VGet(0, rotation - static_cast<float>((M_PI / 180) * 90), 0));
	}


	//正規化
	if (VSquareSize(direction_) > 0)
	{
		direction_ = VNorm(direction_);
	}

	SetLightDirection(VGet(direction.x, 0, direction.x * constant));

	if (input_count != 0)
	{
		target_rot_ = (rotation + (rot / input_count));
		before_rot_ = rot / input_count;
	}

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
	float rot_num = (static_cast<float>((M_PI / 180) * 4)) * (delta_time_ * 10);

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
	fall_speed_ -= (kGravity * delta_time_);

	//地面にいるかの判定
	is_ground_ = CheckGround();


	if (is_ground_)
	{

		if (key_input_[KeyConfig::kJumpKey] || pad_input_.Buttons[PadConfig::kJumpButton])
		{
			//ジャンプの処理
			fall_speed_ = kJumpPower;
			is_ground_ = FALSE;
			now_type_ = AnimationType::kJumpUp;
		}
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

MATRIX Player::GetFrameMatrix()
{
	return MV1GetFrameLocalWorldMatrix(model_,frame_num_);
}

