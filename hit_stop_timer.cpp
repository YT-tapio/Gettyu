#include"DxLib.h"
#include"hit_stop_timer.h"
#include"FPS.h"

HitStopTimer::HitStopTimer()
{

}

void HitStopTimer::TimerUpdate()
{
	timer_ -= (FPS::GetInstance().GetDeltaTime() * 0.1f);
	timer_ = (timer_ < kMin) ? kMin : timer_;
}

void HitStopTimer::SetTime(const float& time)
{
	timer_ = time;
}

bool HitStopTimer::CheckHitStop()
{
	return (timer_ != kMin);
}