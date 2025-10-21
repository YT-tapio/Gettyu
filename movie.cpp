#include"DxLib.h"
#include"movie.h"
#include"screen.h"
MoviePlayer::MoviePlayer(const char* path,bool loop)
{
	handle_		= LoadGraph(path);
	loop_ = loop;
}


MoviePlayer::~MoviePlayer()
{
	DeleteGraph(handle_);
}


void MoviePlayer::Play()
{
	if (loop_)
	{
		PlayMovieToGraph(handle_, DX_PLAYTYPE_LOOP);
	}
	else
	{
		PlayMovieToGraph(handle_);
	}
}

void MoviePlayer::Draw()
{
	DrawExtendGraph(0, 0, kGameWidth, kGameHeight, handle_, TRUE);
}