#include<iostream>
#include"dxlib.h"
#include"game_goal_ui.h"
#include"normal_sub_screen.h"
#include"Draw2D.h"
#include"vector_assistant.h"
#include"offset_assistant.h"
#include"FPS.h"
#include"condition_timer.h"
#include"Font.h"
#include"color.h"

GameGoalUI::GameGoalUI(int* enemy_num)
{

	const int kScreenWidth	= 600;
	const int kScreenHeight = 300;

	//もともとのサイズの2倍する
	const int kInitSize = 2;

	//Screenの全体のサイズ
	int screen_all_size = kScreenWidth + kScreenHeight;

	screen_width_ratio_		= float(kScreenWidth) / screen_all_size;
	screen_height_ratio_	= float(kScreenHeight) / screen_all_size;

	screen_width_		= kScreenWidth * kInitSize;
	screen_height_		= kScreenHeight * kInitSize;

	pos_			= VectorAssistant::GetScreenCenterPos();
	screen_param_	= 0;

	enemy_num_ = enemy_num;
	change_offset_ = FALSE;

	screen_			= std::make_shared<NormalSubScreen>(VectorAssistant::GetZeroVec(), kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, TRUE, AlphaColorType::kWhite, 10, TRUE);
	screen_->SetIsDisp(TRUE);

	const float kDispTime = 1.5f;

	disp_timer_ = std::make_shared<ConditionTimer>(kDispTime);

	const char* kFontPath		= "data/font/TanueiKakuPop_1_00/TanueiKakuPop.otf";
	const char* kFontName	= "たぬえいカクポップタイ";

	const int kFontSize		= 100;
	const int kThick			= 30;
	const int kFontType = DX_FONTTYPE_EDGE;

	font_ = std::make_shared<Font>(kFontPath, kFontName, kFontSize, kThick, kFontType);
}

GameGoalUI::~GameGoalUI()
{
	
}

void GameGoalUI::UpdateScreenSize()
{
	// Screenのサイズや透過率を変える
	// 最終的なサイズ
	
	const int kOffsetSize = 1000;

	const int kScreenWidthMin = kOffsetSize * screen_width_ratio_;
	const int kScreenHeightMin = kOffsetSize * screen_height_ratio_;

	const float kParamMax = 255;
	float offset_param_speed;

	if (screen_param_ == kParamMax) { change_offset_ = TRUE; }

	if (change_offset_)
	{
		disp_timer_->Update();

		if (disp_timer_->GetIsEnd())
		{
			offset_param_speed = (10.f * FPS::GetInstance().GetDeltaTime());
			const float kParamMin = 0;
			OffsetAssistant::Smallf(screen_param_, kParamMin, offset_param_speed);
		}
	}
	else
	{

		offset_param_speed = 10.f * FPS::GetInstance().GetDeltaTime();
		float kOffsetSpeed = 30.f * FPS::GetInstance().GetDeltaTime();

		float width_offset_speed_ = kOffsetSpeed * screen_width_ratio_;
		float height_offset_speed = kOffsetSpeed * screen_height_ratio_;


		OffsetAssistant::Small(screen_width_, kScreenWidthMin, width_offset_speed_);
		OffsetAssistant::Small(screen_height_, kScreenHeightMin, height_offset_speed);
		OffsetAssistant::Bigf(screen_param_, kParamMax, offset_param_speed);
	}
}

void GameGoalUI::DrawScreenObject()
{

	screen_->Up();

	// 文字の描画
	DrawFormatStringToHandle(0, 100, Color::kYellow, font_->GetHandle(), "%d体つかまえろ", *enemy_num_);

	screen_->Down();
}

void GameGoalUI::Update()
{
	// だんだんと小さく、描画も
	UpdateScreenSize();

	DrawScreenObject();

}

void GameGoalUI::Draw()
{
	Draw2D::BlendGraph(pos_, screen_width_, screen_height_, screen_->GetHandle(), TRUE, static_cast<int>(screen_param_));
}