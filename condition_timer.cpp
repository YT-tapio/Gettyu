#include"condition_timer.h"
#include"FPS.h"
#include"debug.h"

ConditionTimer::ConditionTimer(float max_time)
	: max_time_(max_time)
	, timer_(0.f)
	, is_stop_(FALSE)
{

}


ConditionTimer::~ConditionTimer()
{

}



void ConditionTimer::Update()
{
	if (is_stop_) { return; }
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

void ConditionTimer::Start()
{
	is_stop_ = FALSE;
}

void ConditionTimer::Stop()
{
	is_stop_ = TRUE;
}

float ConditionTimer::GetTimeRatio()
{
	return timer_ / max_time_;
}

const float ConditionTimer::GetNowTimer() const
{
	return timer_;
}


bool ConditionTimer::GetIsEnd()
{
	if (timer_ >= max_time_)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

const bool ConditionTimer::GetIsStop() const
{
	return is_stop_;
}


void ConditionTimer::Debug()
{
	DrawFormatString(0, Debug::GetInstance().GetCurrentNum() * 20, GetColor(255, 255, 255), "timer:%.2f", timer_);
	Debug::GetInstance().Add();
}