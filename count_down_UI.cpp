#include<iostream>
#include"DxLib.h"
#include"count_down_UI.h"
#include"normal_sub_screen.h"
#include"vector_assistant.h"
#include"Draw2D.h"
#include"color.h"


CountDownUI::CountDownUI()
{
	const int kScreenWidth		= 200;
	const int kScreenHeight		= 200;

	screen_width_ = kScreenWidth;
	screen_height_ = kScreenHeight;

	pos_ = VectorAssistant::GetScreenCenterPos();
	param_ = 255;
	screen_ = std::make_shared<NormalSubScreen>(pos_, kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	screen_->SetIsDisp(TRUE);
}

CountDownUI::~CountDownUI()
{

}

void CountDownUI::Update(const float& time)
{
	const int kMaxTime = 10;
	int count_down_time = time;

	screen_->Up();

	if (count_down_time > 6)
	{
		if (kMaxTime - time < 0.3f)
		{
			DrawString(0, 0,  "start", Color::kWhite);
		}
		else
		{
			DrawFormatString(0, 0, Color::kWhite, "%d", 10 - count_down_time);
		}

		
	}

	screen_->Down();
}

void CountDownUI::Draw()
{
	Draw2D::BlendGraph(pos_, screen_width_, screen_height_, screen_->GetHandle(), TRUE, param_);
}