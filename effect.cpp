#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"effect.h"


Effect::Effect(const char* file_path, const VECTOR& pos, const VECTOR& rot, 
	float size, float count_max, bool loop)
	: pos_(pos)
	, rot_(rot)
	,handle_(-1)
	,playing_handle_(-1)
	,play_count_(0.f)
	,play_count_max_(count_max)
	,delta_time_(0.f)
	,size_(size)
	,is_play_(FALSE)
	,loop_(loop)
	,is_end_(FALSE)
{
	handle_ = LoadEffekseerEffect(file_path, size_);

	if (handle_ == -1)
	{
		printfDx("effect読み込み失敗");
	}

}

Effect::~Effect()
{
	DeleteEffekseerEffect(handle_);
}


void Effect::Play()
{
	
	//再生していないときは再生させる
	if (!is_play_)
	{
		playing_handle_ = PlayEffekseer3DEffect(handle_);
		SetRotationPlayingEffekseer3DEffect(playing_handle_, rot_.x, rot_.y, rot_.z);
		is_play_ = TRUE;
		play_count_ = 0.0f;
	}
	else
	{

	}

	if (!is_end_ && is_play_)
	{
		play_count_ += 1 * delta_time_;
		// 再生中のエフェクトを移動する。
		SetPosPlayingEffekseer3DEffect(playing_handle_, pos_.x, pos_.y, pos_.z);
	}
	
	//ループなしの場合
	if (!loop_)
	{
		printfDx("%f\n",play_count_);

		if (play_count_ > play_count_max_)
		{
			StopEffekseer3DEffect(playing_handle_);
			is_play_ = FALSE;
			is_end_ = TRUE;
		}
	}
}


void Effect::End()
{
	if (is_play_)
	{
		StopEffekseer3DEffect(playing_handle_);
	}
	
}
