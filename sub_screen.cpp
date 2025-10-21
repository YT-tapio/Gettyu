#include"sub_screen.h"
#include"screen.h"

ConcentrationLine::ConcentrationLine(const int width, const int height,bool alpha)
	:BaseSubScreen(VGet((kGameWidth * 0.5f), (kGameHeight * 0.5f), 0.f),width,height,TRUE,AlphaColorType::kBlack,30)
{
	in_line_ = std::make_shared<MoviePlayer>("data/movie/101594_1280x720.mp4", TRUE);
}

ConcentrationLine::~ConcentrationLine()
{

}


void ConcentrationLine::Update()
{
	if (!is_disp_) 
	{ 
		return;
	}

	Up();
	in_line_->Play();
	in_line_->Draw();
	Down();
}

