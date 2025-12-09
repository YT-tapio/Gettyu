#include"time.h"
#include"FPS.h"
#include"debug.h"

Timer::Timer()
	:time_(0.f)
{

}


void Timer::Update()
{
	time_ += (FPS::GetInstance().GetDeltaTime() * 0.1f);
}


void Timer::Debug()
{
	if (!Debug::GetInstance().GetDisp()) { return; }
	
	DrawFormatString(0, (Debug::GetInstance().GetCurrentNum() * 20), GetColor(255, 255, 255), "timer: %.2f", time_);
	Debug::GetInstance().Add();
}