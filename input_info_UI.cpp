#include"DxLib.h"
#include"input_info_UI.h"
#include"input.h"
#include"keyconfig.h"
#include"Draw2D.h"
#include"FPS.h"

InputInfoUI::InputInfoUI()
{
	const char* kInputInfoPath = "data/image/pad/input_info2.png";
	const char* kButtonPath = "data/image/pad/start_button.png";
	const char* kInputTypeInfoPath = "data/image/input_type_info.png";
	const char* kShutInfoPath = "data/image/shut_info.png";
	input_info_handle_				= LoadGraph(kInputInfoPath);
	button_handle_					= LoadGraph(kButtonPath);
	input_type_info_handle_		= LoadGraph(kInputTypeInfoPath);
	shut_info_handle_				= LoadGraph(kShutInfoPath);
	if (input_info_handle_ == -1)
	{
		printfDx("読み込みエラー\n");
	}
	if (button_handle_ == -1)
	{
		printfDx("読み込みエラー\n");
	}
	if (input_type_info_handle_ == -1)
	{
		printfDx("読み込みエラー\n");
	}
	if (shut_info_handle_ == -1)
	{
		printfDx("読み込みエラー\n");
	}

	is_disp_ = FALSE;
	blend_param_ = 0.f;
}

InputInfoUI::~InputInfoUI()
{
	DeleteGraph(input_info_handle_);
}

void InputInfoUI::ChangeParam()
{
	float delta_time = FPS::GetInstance().GetDeltaTime();

	if (is_disp_)
	{
		blend_param_ += (delta_time * 50.f);

		if (blend_param_ > 255)
		{
			blend_param_ = 255;
		}
	}
	else
	{
		blend_param_ -= (delta_time * 40.f);

		if (blend_param_ < 0)
		{
			blend_param_ = 0;
		}
	}



}

void InputInfoUI::Update()
{
	// プレイヤーの入力によって変化

	if (Input::GetInstance().CheckInputPadButton(PadConfig::kInputInfoButton) == InputState::kPush)
	{
		is_disp_ = (is_disp_) ? FALSE : TRUE;
		/*
		if (is_disp_)
		{
			is_disp_ = FALSE;
		}
		else
		{
			is_disp_ = TRUE;
		}
		*/
	}

	ChangeParam();

}

void InputInfoUI::Draw()
{
	// 後ろに黒背景
	if (blend_param_ != 0.f)
	{
		Draw2D::BlendBox(kInputInfoImagePos, static_cast<int>(kOriginalSize.x * kScale.x), static_cast<int>(kOriginalSize.y * kScale.y), GetColor(0, 0, 10), TRUE, 100);
		Draw2D::BlendGraph(kInputInfoImagePos, static_cast<int>(kOriginalSize.x * kScale.x), static_cast<int>(kOriginalSize.y * kScale.y), input_info_handle_, TRUE, static_cast<int>(blend_param_));
		Draw2D::BlendGraph(kInputInfoShutButtonPos, static_cast<int>(kButtonOriginalSize.x * kShutButtonScale.x), static_cast<int>(kButtonOriginalSize.y * kShutButtonScale.y), button_handle_, TRUE, static_cast<int>(blend_param_));
		Draw2D::BlendGraph(kShutInfoPos, static_cast<int>(kShutOriginalSize.x * kShutScale.x), static_cast<int>(kShutOriginalSize.y * kShutScale.y), shut_info_handle_, TRUE, static_cast<int>(blend_param_));
	}
	else
	{
		Draw2D::ExtendGraph(kInputInfoButtonPos, static_cast<int>(kButtonOriginalSize.x * kButtonScale.x), static_cast<int>(kButtonOriginalSize.y * kButtonScale.y), button_handle_, TRUE);
		Draw2D::ExtendGraph(kInputTypeInfoPos, static_cast<int>(kInputTypeInfoOriginalSize.x * kInputTypeInfoScale.x), static_cast<int>(kInputTypeInfoOriginalSize.y * kInputTypeInfoScale.y), input_type_info_handle_, TRUE);
		//Draw2D::ExtendGraph(kInputInfoButtonPos, static_cast<int>(kButtonOriginalSize.x * kButtonScale.x), static_cast<int>(kButtonOriginalSize.y * kButtonScale.y), input_info_handle_, TRUE);
	}
	

}
