#include"player.h"
#include"keyconfig.h"

#define _USE_MATH_DEFINES
#include <math.h>


Player::Player(VECTOR pos, int model,int pad_num)
	: model_(model)
	, pad_input_num_(pad_num)
{
	//model_ = model;
	//pad_input_num_ = pad_num;
	Init(pos);
}

Player::~Player()
{

}


void Player::Init(VECTOR pos)
{
	/*
	if (!(now_type_ == kNothing))
	{
		animation_.Detach(now_type_);
		now_type_ = kNothing;
	}
	*/
	
	now_type_ = AnimationType::kIdle;

	//animation_.Attach(now_type_);

	before_type_ = AnimationType::kNothing;
	before_before_type_ = AnimationType::kNothing;
	pos_ = pos;
	fall_speed_ = 0.0f;
	velocity_ = VGet(0, 0, 0);
	direction_ = VGet(0, 0, 0);
	rotation_ = VGet(0, 0, 0);
	is_ground_ = TRUE;
}


void Player::Draw()
{
	//MV1SetPosition(model_, pos_);

	MATRIX pos_matrix = MGetTranslate(pos_);
	MATRIX rotation_matrix = MGetRotY(rotation_.y);

	MATRIX model_matrix = MMult(MMult(
		MGetRotY(rotation_.y), MGetScale(VGet(0.01f, 0.01f, 0.01f))),pos_matrix);

	//MV1SetRotationXYZ(model_, rotation_);
	DrawSphere3D(VGet(pos_.x, pos_.y + 15, pos_.z), 0.5f, 5, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

	//MV1SetScale(model_, VGet(0.01f, 0.01f, 0.01f));

	MV1SetMatrix(model_, model_matrix);

	MV1DrawModel(model_);
	animation_.Draw(now_type_);
}


void Player::AddAnim(const AnimationData& animation_data)
{
	animation_.Add(animation_data);
}


void Player::InputState()
{
	GetHitKeyStateAll(key_input_);

	GetJoypadXInputState(pad_input_num_, &pad_input_);
}


void Player::Update(const VECTOR& pos, const float& rotation)
{

	InputState();

	InputMovement(pos, rotation);
	
	
	pos_ = VAdd(pos_, velocity_);
}

void Player::InputMovement(const VECTOR& pos, const float& rotation)
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
		printfDx("Idle");
	}

	if (key_input_[KEY_INPUT_2])
	{
		now_type_ = AnimationType::kWalk;
		printfDx("Walk");
	}

	if (key_input_[KEY_INPUT_3])
	{
		now_type_ = AnimationType::kSlowRun;
		printfDx("SlowRun");
	}

	if (key_input_[KEY_INPUT_4])
	{
		now_type_ = AnimationType::kFastRun;
		printfDx("FastRun");
	}

	/*
	if (now_type_ != AnimationType::kIdle)
	{
		now_type_ = AnimationType::kIdle;
	}
	*/


	if (before_type_ != now_type_)
	{

		if (!(before_type_ == AnimationType::kNothing))
		{
			animation_.InitBlend(now_type_, before_type_);

			if (animation_.GetBlendFlag())
			{
				animation_.Detach(before_before_type_);
			}
		}



		animation_.Attach(now_type_);

		before_before_type_ = before_type_;
		before_type_ = now_type_;

		animation_.SetBlend(TRUE);

	}

	animation_.Update(now_type_);




	velocity_ = VScale(velocity,delta_time_);


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


void Player::CheckDirection(const VECTOR& pos, const float& rotation)
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
			if (key_input_[KeyConfig::kRightKey] || pad_input_.ThumbLX < PadConfig::kRightStick)
			{
				rot += rotation;

			}
			else
			{
				rot += (rotation + static_cast<float>((M_PI / 180) * 360));
			}

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
			rot += (rotation + static_cast<float>((M_PI / 180) * 180));

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
			rot += (rotation + static_cast<float>((M_PI / 180) * 90));
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
			rot += rotation + static_cast<float>((M_PI / 180) * 270);

			input_count++;
		}

		//rotation_ = VAdd(rotation_, VGet(0, rotation - static_cast<float>((M_PI / 180) * 90), 0));
	}


	//正規化
	if (VSquareSize(direction_) > 0)
	{
		direction_ = VNorm(direction_);
	}

	if (input_count != 0)
	{
		rotation_ = VGet(0, rot / input_count, 0);
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