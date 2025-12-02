#include"DxLib.h"
#include"button_selecter.h"
#include"keyconfig.h"
#include"input.h"

ButtonSelecter::ButtonSelecter()
	:is_slide_(FALSE)
{

}

ButtonSelecter::~ButtonSelecter()
{

}

/*-----private---------*/

bool ButtonSelecter::IsInputUp()
{
	//各入力をみる
	for (auto& num : KeyConfig::kSelectUpKey)
	{
		if (Input::GetInstance().CheckInputKey(num) == InputState::kPush) { return TRUE; }
	}
	if (Input::GetInstance().CheckInputPadButton(PadConfig::kSelectUpButton) == InputState::kPush) { return TRUE; }
	if (Input::GetInstance().GetPadMove(StickType::kLeft, Control::kY, PadConfig::kSelectUpStick))
	{
		if (!is_slide_)
		{
			is_slide_ = TRUE;
			return TRUE;
		}

	}
	else
	{
		is_slide_ = FALSE;
	}
	 
	return FALSE;
}

bool ButtonSelecter::IsInputDown()
{
	//各入力をみる
	for (auto& num : KeyConfig::kSelectDownKey)
	{
		if (Input::GetInstance().CheckInputKey(num) == InputState::kPush) { return TRUE; }
	}
	if (Input::GetInstance().CheckInputPadButton(PadConfig::kSelectDownButton) == InputState::kPush) { return TRUE; }
	if (Input::GetInstance().GetPadMove(StickType::kLeft, Control::kY, PadConfig::kSelectDownStick)) 
	{ 
		if (!is_slide_)
		{
			is_slide_ = TRUE;
			return TRUE;
		}
		
	}
	else
	{
		is_slide_ = FALSE;
	}

	return FALSE;
}

bool ButtonSelecter::IsInputRight()
{
	//各入力をみる
	for (auto& num : KeyConfig::kSelectRightKey)
	{
		if (Input::GetInstance().CheckInputKey(num) == InputState::kPush) { return TRUE; }
	}
	if (Input::GetInstance().CheckInputPadButton(PadConfig::kSelectRightButton) == InputState::kPush) { return TRUE; }
	if (Input::GetInstance().GetPadMove(StickType::kLeft, Control::kX, PadConfig::kSelectRightStick))
	{
		if (!is_slide_)
		{
			is_slide_ = TRUE;
			return TRUE;
		}

	}
	else
	{
		is_slide_ = FALSE;
	}

	return FALSE;
}

bool ButtonSelecter::IsInputLeft()
{
	//各入力をみる
	for (auto& num : KeyConfig::kSelectLeftKey)
	{
		if (Input::GetInstance().CheckInputKey(num) == InputState::kPush) { return TRUE; }
	}
	if (Input::GetInstance().CheckInputPadButton(PadConfig::kSelectLeftButton) == InputState::kPush) { return TRUE; }
	if (Input::GetInstance().GetPadMove(StickType::kLeft, Control::kX, PadConfig::kSelectLeftStick))
	{
		if (!is_slide_)
		{
			is_slide_ = TRUE;
			return TRUE;
		}

	}
	else
	{
		is_slide_ = FALSE;
	}

	return FALSE;
}

/*----public---------*/

int ButtonSelecter::Vertical()
{
	int num = 0;

	if (IsInputUp())
	{
		num++;
	}

	if (IsInputDown())
	{
		num--;
	}


	return num;
}

int ButtonSelecter::Side()
{
	int num = 0;

	if (IsInputRight())
	{
		num++;
	}

	if (IsInputLeft())
	{
		num--;
	}

	return num;
}

int ButtonSelecter::Select(SelectType type)
{
	int num = 0;

	if (type == SelectType::kVertical)
	{
		num = num + Vertical();
	}
	else if (type == SelectType::kSide)
	{
		num = num + Side();
	}
	else
	{
		printfDx("エラー\n");
	}
	


	return num;
}