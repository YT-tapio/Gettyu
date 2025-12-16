#include<iostream>
#include"DxLib.h"
#include"count_down_UI.h"
#include"normal_sub_screen.h"
#include"font.h"
#include"vector_assistant.h"
#include"Draw2D.h"
#include"color.h"
#include"FPS.h"
#include"sound.h"
#include"2D_sound.h"

CountDownUI::CountDownUI()
{
	const int kScreenWidth		= 200;
	const int kScreenHeight		= 200;

	const int kStartScreenWidth = 700;
	const int kStartScreenHeight = 200;

	const char* kFontPath	= "data/font/TanueiKakuPop_1_00/TanueiKakuPop.otf";
	const char* kFontName	= "たぬえいカクポップタイ";

	const int kFontSize		= 200;
	const int kThick		= 20;
	const int kFontType		= DX_FONTTYPE_EDGE;
	
	const char* kSoundPath = "data/sound/game/se/start.mp3";
	const int kVolume = 200;

	before_num_ = 10;

	screen_width_		= kScreenWidth;
	screen_height_		= kScreenHeight;
	
	start_ui_screen_width_		= kStartScreenWidth;
	start_ui_screen_height_		= kStartScreenHeight;

	pos_			= VectorAssistant::GetScreenCenterPos();
	param_			= 255;
	screen_			= std::make_shared<NormalSubScreen>(pos_, kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, TRUE, AlphaColorType::kBlack, 10, FALSE);
	start_screen_	= std::make_shared<NormalSubScreen>(pos_, kStartScreenWidth, kStartScreenHeight, kStartScreenWidth, kStartScreenHeight, TRUE, AlphaColorType::kBlack, 10, FALSE);
	screen_->SetIsDisp(TRUE);
	start_screen_->SetIsDisp(FALSE);
	font_			= std::make_shared<Font>(kFontPath, kFontName, kFontSize, kThick, kFontType);

	start_sound_ = std::make_shared<Sound2D>(kSoundPath, DX_PLAYTYPE_BACK, kVolume, FALSE);
}

CountDownUI::~CountDownUI()
{

}

void CountDownUI::Update(const float& time)
{
	const int kMaxTime = 10;
	int count_down_time = time;

	if (before_num_ != count_down_time)
	{
		param_ = kParamMax;
		before_num_ = count_down_time;
	}

	screen_->Up();
	if (count_down_time > 6)
	{
		float offset_param_speed;
		if (kMaxTime - time == 0)
		{
			start_screen_->SetIsDisp(TRUE);
			screen_width_ = 600;
			offset_param_speed = 9.f;
			start_sound_->Update();
		}
		else
		{
			int width = GetDrawFormatStringWidthToHandle(font_->GetHandle(), "%d", kMaxTime - count_down_time);
			int pos_x = static_cast<int>((float(screen_width_) * 0.5f) - float(width) * 0.5f);
			DrawFormatStringToHandle(pos_x, 0, Color::kRed, font_->GetHandle(), "%d", kMaxTime - count_down_time);

			offset_param_speed = 15.f;
		}
		
		param_ -= offset_param_speed * FPS::GetInstance().GetDeltaTime();
	}
	screen_->Down();

	start_screen_->Up();

	DrawStringToHandle(70, 0, "start", Color::kYellow, font_->GetHandle(), Color::kRed);

	start_screen_->Down();

}

void CountDownUI::Draw()
{
	if (screen_->GetIsDisp())
	{
		Draw2D::BlendGraph(pos_, screen_width_, screen_height_, screen_->GetHandle(), TRUE, param_);
	}

	if (start_screen_->GetIsDisp())
	{
		Draw2D::BlendGraph(pos_, start_ui_screen_width_, start_ui_screen_height_, start_screen_->GetHandle(), TRUE, param_);
	}
	
}