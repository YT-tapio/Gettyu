#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"effect.h"


Effect::Effect(const char* file_path, const VECTOR& pos,float size)
	: pos_(pos)
	,handle_(-1)
	,play_handle_(-1)
	,size_(size)
	,is_play_(FALSE)
{
	handle_ = LoadEffekseerEffect(file_path, size_);

	if (handle_ == -1)
	{
		printfDx("effectì«Ç›çûÇ›é∏îs");
	}

}

Effect::~Effect()
{
	DeleteEffekseerEffect(handle_);
}