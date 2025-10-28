#include"condition_timer.h"
#include"FPS.h"
#include"debug.h"

ConditionTimer::ConditionTimer(float max_time)
	: max_time_(max_time)
	, timer_(0.f)
{

}


ConditionTimer::~ConditionTimer()
{

}



void ConditionTimer::Update()
{
	timer_ += (FPS::GetInstance().GetDeltaTime() * 0.1f);

	if (GetTimeRatio() >= 1.f)
	{
		timer_ = max_time_;
	}

}


void ConditionTimer::Reset()
{
	timer_ = 0.f;
}


float ConditionTimer::GetTimeRatio()
{
	return timer_ / max_time_;
}


bool ConditionTimer::GetIsEnd()
{
	if (timer_ == max_time_)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void ConditionTimer::Debug()
{
	DrawFormatString(0, Debug::GetInstance().GetCurrentNum() * 20, GetColor(255, 255, 255), "timer:%.2f", timer_);
}