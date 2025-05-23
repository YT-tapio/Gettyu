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
	DrawSphere3D(pos_, 0.5f, 5, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

	MV1SetScale(model_, VGet(0.1f, 0.1f, 0.1f));

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


void Player::Update()
{

	InputState();

	VECTOR velocity = { 0.0f,0.0f,0.0f };

	float speed = 0.0f;

	direction_ = VGet(0, 0, 0);

	/*(PadConfig::kLeftButton)*/

	if (key_input_[KeyConfig::kRightKey] || pad_input_.ThumbLX > (PadConfig::kLeftButton))
	{
		direction_ = VAdd(direction_, VGet(1, 0, 0));
	}


	if (key_input_[KeyConfig::kLeftKey] || pad_input_.ThumbLX < (PadConfig::kRightButton))
	{
		direction_ = VAdd(direction_, VGet(-1, 0, 0));
	}

	//移動しているなら正規化
	if (VSquareSize(direction_) > 0)
	{
		direction_ = VNorm(direction_);
	}

	velocity = VScale(direction_, 0.5f);


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