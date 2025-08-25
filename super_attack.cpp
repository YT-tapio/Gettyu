#include"DxLib.h"
#include"super_attack.h"

SuperAttack::SuperAttack(const VECTOR& pos,int effect_handle)
	: now_situation_num_(0)
	, pos_(pos)
	, effect_handle_(effect_handle)
	, is_play_(FALSE)
	, play_count_(0.f)
	, max_play_count_(0.f)
	, delta_time_(0.f)
{

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

