#include"FPS.h"
#include<math.h>

FPS::FPS()
	: prev_time_(GetNowHiPerformanceCount())
	, now_time_(0.0f)
	, delta_time_(0.0f)
	, count_(0)
	,now_fps_(0.0f)
	,time_scale_(1.0f)
{

}

FPS::~FPS()
{
	//ˆ—‚È‚µ
}


void FPS::Init()
{
	prev_time_ = GetNowHiPerformanceCount();
	now_time_ = 0.0f;
	delta_time_ = 0.0f;
	count_ = 0;
	now_fps_ = 0.0f;
	time_scale_ = 0.0f;
}


void FPS::Update()
{
	now_time_ = GetNowHiPerformanceCount();

	delta_time_ = ((now_time_ - prev_time_) / 100000.0f) * time_scale_;

	if (count_ == kTargetFps)
	{
		now_fps_ = 1000.0f / 
			((now_time_ - prev_time_) / kTargetFps);

		count_ = 0;
	}

	count_++;


}


void FPS::Wait()
{
	LONGLONG took_time = GetNowHiPerformanceCount() - prev_time_;
	LONGLONG wait_time = count_ * 1000 / kTargetFps - took_time;

	if (wait_time > 0)
	{
		Sleep(wait_time);
	}
}


void FPS::DrawDeltaTime()
{
	DrawFormatString(400, 100, GetColor(255, 255, 255), "%.1f", now_fps_);
}

void FPS::DrawTimeScale()
{
	DrawFormatString(400, 100, GetColor(255, 255, 255), "%f", time_scale_);
}
