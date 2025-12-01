#include<iostream>
#include"button.h"

#include"input.h"
#include"vector_assistant.h"
#include"offset_assistant.h"
#include"keyconfig.h"
#include"collision2D.h"
#include"Draw2D.h"

Button::Button(const VECTOR pos,const float width,const float height,const char* path,const int& num)
	: pos_(pos)
	, init_width_(width)
	, init_height_(height)
	, width_(width)
	, height_(height)
	, is_select_(FALSE)
	, is_pussed_(FALSE)
	, num_(num)
	, width_ratio_(0.f)
	,height_ratio_(0.f)
	, state_(ButtonState::kDefault)
{
	model_ = LoadGraph(path);

	float sum = width + height;

	//選択されたときのspeed
	width_ratio_ = width / sum;
	height_ratio_ = height / sum;

	if (model_ == -1)
	{
		printfDx("画像の読み込み失敗");
	}
}

Button::~Button()
{
	DeleteGraph(model_);
}

/*private----------------------------------*/

bool Button::IsPushConditionMouse()
{
	if (Input::GetInstance().CheckInputMouse(KeyConfig::kSelectMouseButton) == InputState::kPush) { return TRUE; }
	return FALSE;
}

bool Button::IsPushConditionButton()
{
	if (Input::GetInstance().CheckInputKey(KeyConfig::kSelectKey) == InputState::kPush) { return TRUE; }
	if (Input::GetInstance().CheckInputPadButton(KeyConfig::kSelectMouseButton) == InputState::kPush) { return TRUE; }
	return FALSE;
}

bool Button::IsReleaseCondition()
{
	if (Input::GetInstance().CheckInputMouse(KeyConfig::kSelectMouseButton) == InputState::kRelease)		{ return TRUE; }
	return FALSE;
}

void Button::IsOnMouse(const int& num)
{
	width_		= init_width_;
	height_		= init_height_;
	//自分と同じの時は早期リターン
	if (num == num_)
	{
		state_ = ButtonState::kSelect;
		return;
	}

	//同じじゃないときはmouse_posが自分の場所にいるかの判断を行う

	// 押したかの判断
	VECTOR mouse_pos = VectorAssistant::Get2DVec(static_cast<float>(Input::GetInstance().GetMousePosX()),
		static_cast<float>(Input::GetInstance().GetMousePosY()));

	
	state_ = (Collision2D::IsInBox(mouse_pos, pos_, width_, height_)) ? ButtonState::kSelect : state_;
}



void Button::SelectUpdate()
{
	const float kOffsetSize		= 20.f;

	float target_width			= init_width_ + (kOffsetSize * width_ratio_);
	float target_height			= init_height_ + (kOffsetSize * height_ratio_);

	//大きくする処理をはさむ
	OffsetAssistant::UniformBigf(width_, target_width,kSpeed);
	OffsetAssistant::UniformBigf(height_, target_height,kSpeed);

	if (IsPushConditionMouse())
	{
		state_ = ButtonState::kPressed;
	}

	if (IsPushConditionButton())
	{
		//ボタンを押したという判定になる

	}


}

void Button::PressedUpdate()
{
	//このなかでbuttonの範囲内で離されたならその選択は除外される

	// 押したかの判断
	VECTOR mouse_pos = VectorAssistant::Get2DVec(static_cast<float>(Input::GetInstance().GetMousePosX()),
		static_cast<float>(Input::GetInstance().GetMousePosY()));
	//boxの範囲内の検出
	if (Collision2D::IsInBox(mouse_pos, pos_, width_, height_))
	{
		//離した判定になる
		if(Input::GetInstance().CheckInputMouse(KeyConfig::kSelectMouseButton) == InputState::kRelease) 
		{
			//押したというような判定になる
		}
	}
	else
	{
		state_ = ButtonState::kDefault;
	}


}

/*public---------------------------------*/

void Button::Update(const int& num)
{
	//自分が選択状態じゃないなら
	if (num != num_)
	{
		state_ = ButtonState::kDefault;
	}

	switch (state_)
	{
	case ButtonState::kDefault:
		IsOnMouse(num);
		printfDx("Defaults\n");
		break;

	case ButtonState::kSelect:
		SelectUpdate();
		printfDx("Select\n");
		break;

	case ButtonState::kPressed:
		PressedUpdate();
		printfDx("Pressed\n");
		break;

	}
}


void Button::Draw()
{
	if (model_ == -1)
	{
		Draw2D::Box(pos_, static_cast<int>(width_), static_cast<int>(height_), GetColor(255, 255, 255), TRUE);
	}
	else
	{
		Draw2D::ExtendGraph(pos_, static_cast<int>(width_), static_cast<int>(height_), model_, TRUE);
	}

}