#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"super_attack.h"


SuperAttack::SuperAttack(const VECTOR& pos,const char*  file_path)
	: now_situation_num_(0)
	, pos_(pos)
	, effect_handle_(-1)
	, is_play_(FALSE)
	, play_count_(0.f)
	, max_play_count_(0.f)
	, delta_time_(0.f)
{
	// エフェクトのリソースを読み込む
	effect_handle_ = LoadEffekseerEffect(file_path, 1.0f);

	if (effect_handle_ == -1)
	{
		printfDx("失敗");
	}
}


SuperAttack::~SuperAttack()
{
	
}


void SuperAttack::Update()
{

	play_count_ += (1 * (delta_time_ * 10));

	if (!is_play_)
	{
		is_play_ = TRUE;
	}
	else if (max_play_count_ < play_count_)
	{
		play_count_ = 0.f;
		is_play_ = FALSE;
	}

}

void SuperAttack::Draw()
{
	if (effect_handle_ == -1)
	{
		DrawSphere3D(pos_, 10, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}
	else
	{

	}


}

