#include<iostream>
#include"lerp.h"
#include"FPS.h"


VECTOR Lerp(const VECTOR& start_pos,const VECTOR& now_pos, const VECTOR& target_pos, const float max_time, float& timer, bool& flag)
{
	//最終的に返すvelocity
	VECTOR vel = VGet(0.f, 0.f, 0.f);

	//まず保管を開始した場所から行きたいところまでの距離はかる

	VECTOR dist = VSub(target_pos, start_pos);

	//タイマーをカウント(俺のdelta_timeは一桁ずれてしまっているので0.1をかけて調整している)
	timer += (FPS::GetInstance().GetDeltaTime() * 0.1f);
	
	if (timer > max_time)
	{
		timer = max_time;
		flag = FALSE;
	}


	//時間の比を作る
	float time_ratio = 1.f;

	//0を除外
	if (max_time != 0)
	{
		time_ratio = timer / max_time;
	}

	VECTOR test = VScale(dist, time_ratio);

	//比をdistにかけてそれを保管を開始した位置にタス
	VECTOR next_pos = VAdd(start_pos,test);


	vel = VSub(next_pos, now_pos);

	return vel;
}