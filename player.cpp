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
	pos_ = pos;
	velocity_ = VGet(0, 0, 0);
	direction_ = VGet(0, 0, 0);
}


void Player::Draw()
{
	DrawSphere3D(pos_, 0.5f, 5, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
	MV1DrawModel(model_);
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
		direction_ = VAdd(direction_, VGet(0, 0, 0));


	}


	if (key_input_[KeyConfig::kLeftKey] || pad_input_.ThumbLX < (PadConfig::kRightButton))
	{
		direction_ = VAdd(direction_, VGet(0, 0, 0));


	}

	//ˆÚ“®‚µ‚Ä‚¢‚é‚È‚ç³‹K‰»
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

	pos_ = VAdd(pos_, velocity_);


	MV1SetPosition(model_, pos_);
}