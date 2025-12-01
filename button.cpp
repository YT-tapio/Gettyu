#include<iostream>
#include"button.h"

#include"input.h"
#include"vector_assistant.h"
#include"keyconfig.h"
#include"collision2D.h"
#include"Draw2D.h"

Button::Button(const VECTOR pos,const int width,const int height,const char* path,const int& num)
	: pos_(pos)
	, width_(width)
	, height_(height)
	, is_select_(FALSE)
	, is_push_(FALSE)
	,num_(num)
{
	model_ = LoadGraph(path);

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

bool Button::IsPushCondition()
{
	//選択のkey
	if (Input::GetInstance().CheckInputKey(KeyConfig::kSelectKey) == InputState::kPush)					{ return TRUE; }
	if (Input::GetInstance().CheckInputMouse(KeyConfig::kSelectMouseButton) == InputState::kPush)		{ return TRUE; }
	if (Input::GetInstance().CheckInputPadButton(KeyConfig::kSelectMouseButton) == InputState::kPush)	{ return TRUE; }
	return FALSE;
}


bool Button::IsReleaseCondition()
{
	if (Input::GetInstance().CheckInputKey(KeyConfig::kSelectKey) == InputState::kRelease)					{ return TRUE; }
	if (Input::GetInstance().CheckInputMouse(KeyConfig::kSelectMouseButton) == InputState::kRelease)		{ return TRUE; }
	if (Input::GetInstance().CheckInputPadButton(KeyConfig::kSelectMouseButton) == InputState::kRelease)	{ return TRUE; }
	return FALSE;
	
}

bool Button::IsOnMouse(const int& num)
{
	//自分と同じの時は早期リターン
	if (num == num_)
	{
		return TRUE;
	}

	//同じじゃないときはmouse_posが自分の場所にいるかの判断を行う

	// 押したかの判断
	VECTOR mouse_pos = VectorAssistant::Get2DVec(static_cast<float>(Input::GetInstance().GetMousePosX()),
		static_cast<float>(Input::GetInstance().GetMousePosY()));

	

	return Collision2D::IsInBox(mouse_pos, pos_, width_, height_);
}



bool Button::IsPush()
{

	if (!is_select_) { return FALSE; }


	// この時にpushしたら
	if (IsPushCondition())
	{
		return TRUE;
	}

	return FALSE;
}

void Button::IsPushUpdate()
{
	//実行されないようにする

	

}

/*public---------------------------------*/

void Button::Update(const int& num)
{
	is_select_ = IsOnMouse(num);

	//セレクトされていないときは早期リターン
	if (!is_select_) 
	{ 
		is_push_ = FALSE;

		//選択されていないときは画像をもとのサイズに戻す
		//printfDx("選択されていない\n");
	}
	else
	{
		is_push_ = IsPush();
		//printfDx("選択されている\n");
	}

	// 押されているとき時の
	


}


void Button::Draw()
{
	if (model_ == -1)
	{
		Draw2D::Box(pos_, width_, height_, GetColor(255, 255, 255), TRUE);
	}
	else
	{
		Draw2D::ExtendGraph(pos_, width_, height_, model_, TRUE);
	}

}