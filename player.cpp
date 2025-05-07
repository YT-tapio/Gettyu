#include"player.h"

Player::Player(VECTOR pos, int model)
{
	model_ = model;
	Init(pos);
}

Player::~Player()
{

}


void Player::Init(VECTOR pos)
{
	pos_ = pos;
}


void Player::Draw()
{
	DrawSphere3D(pos_, 0.5f, 5, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
	MV1DrawModel(model_);
}


void Player::Update()
{
	MV1SetPosition(model_, pos_);
}