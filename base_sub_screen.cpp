#include"base_sub_screen.h"

BaseSubScreen::BaseSubScreen(const VECTOR& pos,const int width, const int height, bool alpha, AlphaColorType color_type,const int param)
{
	handle_			= MakeScreen(width, height, alpha);
	is_disp_		= FALSE;

	screen_width_	= width;
	screen_height_	= height;

	center_pos_ = pos;
	color_type_ = color_type;
	param_ = param;
}

BaseSubScreen::~BaseSubScreen()
{

}


void BaseSubScreen::Up()
{
	//‚±‚Ì‰æ–Ê‚ð‹N“®‚·‚é
	SetDrawScreen(handle_);
}


void BaseSubScreen::Down()
{
	//Œ³‚Ì‰æ–Ê‚É–ß‚·
	SetDrawScreen(DX_SCREEN_BACK);
}


void BaseSubScreen::Draw()
{
	if (!is_disp_)
	{
		return;
	}

	if (color_type_ == AlphaColorType::kBlack)
	{
		GraphFilter(handle_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, param_, TRUE, GetColor(0, 255, 0), 0);
	}
	else
	{
		GraphFilter(handle_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_GREATER, param_, TRUE, GetColor(0, 255, 0), 0);
	}

	
	DrawExtendGraph(static_cast<float>(center_pos_.x - (screen_width_ * 0.5f)),
		static_cast<float>(center_pos_.y - (screen_height_ * 0.5f)), 
		static_cast<float>(center_pos_.x + (screen_width_ * 0.5f)),
		static_cast<float>(center_pos_.y + (screen_height_ * 0.5f)),
		handle_, TRUE);
}

void BaseSubScreen::SetIsDisp(const bool& flag)
{
	is_disp_ = flag;
}