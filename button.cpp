#include<iostream>
#include"button.h"

#include"input.h"
#include"vector_assistant.h"
#include"offset_assistant.h"
#include"keyconfig.h"
#include"collision2D.h"
#include"Draw2D.h"

Button::Button(const VECTOR pos,const float width,const float height,const char* path,const int& num,bool* flag)
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
	,flag_(flag)
{
	debug_color_ = kWhite;
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
	if (model_ != -1)
	{
		DeleteGraph(model_);
		model_ = -1;
	}
}

/*private----------------------------------*/

bool Button::IsPushConditionMouse()
{
	if (Input::GetInstance().CheckInputMouse(KeyConfig::kSelectMouseButton) == InputState::kPush) { return TRUE; }
	return FALSE;
}

bool Button::IsPushConditionButton()
{
	for (auto& num : KeyConfig::kSelectKey)
	{
		if (Input::GetInstance().CheckInputKey(num) == InputState::kPush) { return TRUE; }
	}
	if (Input::GetInstance().CheckInputPadButton(PadConfig::kSelectButton) == InputState::kPush) { return TRUE; }
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
	VECTOR mouse_pos = VectorAssistant::Get2DVec(static_cast<float>(Input::GetInstance().GetMousePosX()),
		static_cast<float>(Input::GetInstance().GetMousePosY()));

	//マウスが動いていないなら早期リターン
	if (!(Input::GetInstance().GetMouseMove())) 
	{
		//もしボタンをクリックしたならこいつに変える
		if (Collision2D::IsInBox(mouse_pos, pos_, width_, height_))
		{
			if (IsPushConditionMouse())
			{
				state_ = ButtonState::kPressed;
			}
		}
		return;
	}

	state_ = (Collision2D::IsInBox(mouse_pos, pos_, width_, height_)) ? ButtonState::kSelect : state_;
}



void Button::SelectUpdate()
{
	const float kOffsetSize		= 20.f;

	float target_width			= init_width_ + (kOffsetSize * width_ratio_);
	float target_height			= init_height_ + (kOffsetSize * height_ratio_);

	float speed = kSpeed * FPS::GetInstance().GetDeltaTime();

	//大きくする処理をはさむ
	OffsetAssistant::Bigf(width_, target_width,speed);
	OffsetAssistant::Bigf(height_, target_height,speed);

	if (IsPushConditionMouse())
	{
		state_ = ButtonState::kPressed;
	}

	if (IsPushConditionButton())
	{
		//ボタンを押したという判定になる
		*flag_ = TRUE;
	}


}

void Button::PressedUpdate()
{
	const float kOffsetSize = 20.f;

	float target_width = init_width_ + (kOffsetSize * width_ratio_);
	float target_height = init_height_ + (kOffsetSize * height_ratio_);

	float speed = kSpeed * FPS::GetInstance().GetDeltaTime();

	// 大きくする処理をはさむ
	OffsetAssistant::Bigf(width_, target_width, speed);
	OffsetAssistant::Bigf(height_, target_height, speed);

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
			*flag_ = TRUE;
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
		//printfDx("Defaults\n");
		debug_color_ = kWhite;
		break;

	case ButtonState::kSelect:
		SelectUpdate();
		//printfDx("Select\n");
		debug_color_ = kRightGray;
		break;

	case ButtonState::kPressed:
		PressedUpdate();
		//printfDx("Pressed\n");
		debug_color_ = kGray;
		break;

	}
}


void Button::Draw()
{
	
	if (model_ == -1)
	{	
		Draw2D::Box(pos_, static_cast<int>(width_), static_cast<int>(height_), kWhite, TRUE);
	}
	else
	{
		
		Draw2D::ExtendGraph(pos_, static_cast<int>(width_), static_cast<int>(height_), model_, TRUE);
	}

	//選択しているものは上から倒壊した物をかぶせる
	if (state_ != ButtonState::kDefault) { Draw2D::BlendBox(pos_, static_cast<int>(width_), static_cast<int>(height_), debug_color_, TRUE, 128); }

}