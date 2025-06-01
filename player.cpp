#include"player.h"
#include"keyconfig.h"



Player::Player(VECTOR pos, int model,int pad_num)
{
	model_ = model;
	pad_input_num_ = pad_num;
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
	velocity_ = VGet(0, 0, 0);
	direction_ = VGet(0, 0, 0);
}


void Player::Draw()
{
	MV1SetPosition(model_, pos_);
	DrawSphere3D(VGet(pos_.x, pos_.y + 15, pos_.z), 0.5f, 5, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

	MV1SetScale(model_, VGet(0.01f, 0.01f, 0.01f));

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


void Player::Update(const VECTOR& pos)
{

	InputState();

	VECTOR velocity = { 0.0f,0.0f,0.0f };

	float speed = 0.0f;

	direction_ = VGet(0, 0, 0);

	/*(PadConfig::kLeftButton)*/

	CheckDirection(pos);

	velocity = VScale(direction_, speed_);


	if (VSize(velocity) != 0)
	{
		direction_ = VNorm(velocity);
	}

	velocity_ = velocity;

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



	pos_ = VAdd(pos_, velocity_);


	
}


void Player::CheckDirection(const VECTOR& pos)
{
	float constant = 0.0f;

	MakeLine(constant, pos);

	VECTOR direction = VGet(0, 0, 0);



	//どちらが前かの判別
	//原点からの距離を見る

	float my_scale = sqrt((pos_.x * pos_.x) + (pos_.z * pos_.z));
	float other_scale = sqrt((pos.x * pos.x) + (pos.z * pos.z));

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
	if (key_input_[KeyConfig::kUpKey])
	{
		direction_ = VAdd(direction_, VGet(direction.x, 0, direction.x * constant));
	}

	//後ろ
	if (key_input_[KeyConfig::kDownKey])
	{
		direction_ = VAdd(direction_, VGet(-1.0f * (direction.x), 0, -1.0f * (direction.x * constant)));
	}


	//右
	if (key_input_[KeyConfig::kRightKey])
	{
		direction_ = VAdd(direction_, VGet(direction.x * constant, 0, -direction.x));
	}

	//左
	if (key_input_[KeyConfig::kLeftKey])
	{
		direction_ = VAdd(direction_, VGet(-(direction.x * constant), 0, direction.x));
	}


	//正規化
	if (VSquareSize(direction_) > 0)
	{
		direction_ = VNorm(direction_);
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