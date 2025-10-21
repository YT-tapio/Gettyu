#include"sub_screen.h"
#include"screen.h"
BaseScreen::BaseScreen(const int width, const int height,bool alpha)
{
	handle_ = MakeScreen(width, height, alpha);
	in_line_ = std::make_shared<MoviePlayer>("data/movie/101594_1280x720.mp4", TRUE);
}

BaseScreen::~BaseScreen()
{

}


void BaseScreen::Up()
{
	//‚±‚Ì‰æ–Ê‚ð‹N“®‚·‚é
	SetDrawScreen(handle_);
}


void BaseScreen::Down()
{
	//Œ³‚Ì‰æ–Ê‚É–ß‚·
	SetDrawScreen(DX_SCREEN_BACK);
}


void BaseScreen::Update()
{
	Up();
	in_line_->Play();
	in_line_->Draw();
	Down();
}


void BaseScreen::Draw()
{
	GraphFilter(handle_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 100, TRUE, GetColor(0, 255, 0), 0);
	DrawExtendGraph(0, 0, kGameWidth, kGameHeight, handle_, TRUE);
}