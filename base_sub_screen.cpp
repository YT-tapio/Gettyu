#include"base_sub_screen.h"

BaseSubScreen::BaseSubScreen(const VECTOR& pos,const int screen_width, const int screen_height, const int width,const int height,bool alpha, AlphaColorType color_type,const int param, bool is_blend)
{
	handle_			= MakeScreen(screen_width, screen_height, alpha);
	is_disp_		= FALSE;

	screen_width_	= width;
	screen_height_	= height;

	center_pos_ = pos;
	color_type_ = color_type;
	param_ = param;
	is_blend_ = is_blend_;
}

BaseSubScreen::~BaseSubScreen()
{

}


void BaseSubScreen::Up()
{
	//‚±‚Ì‰æ–Ê‚ð‹N“®‚·‚é
	SetDrawScreen(handle_);
	ClearDrawScreen();
}


void BaseSubScreen::Down()
{
	//Œ³‚Ì‰æ–Ê‚É–ß‚·
	SetDrawScreen(DX_SCREEN_BACK);
}


void BaseSubScreen::Draw()
{

	if (is_blend_)
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