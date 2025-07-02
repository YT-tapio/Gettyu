#include<iostream>
#include"DxLib.h"
#include"camera.h"
#include"turn_camera.h"

TurnCamera::TurnCamera(const VECTOR& pos)
	:position_(pos)
	,change_type(ChangeType::Turn)
{

}


TurnCamera::~TurnCamera()
{

}

void TurnCamera::Init(const VECTOR& pos)
{
	position_ = pos;
}