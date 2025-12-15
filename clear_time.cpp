#include"DxLib.h"
#include"clear_time.h"
#include"FPS.h"

ClearTime::ClearTime()
{

}

void ClearTime::Update()
{
	if (is_stop_) { return; }

	clear_time_ += (FPS::GetInstance().GetDeltaTime() * 0.1f);
}

void ClearTime::Stop()
{
	is_stop_ = TRUE;
}

void ClearTime::Reset()
{
	clear_time_ = 0.f;
}

void ClearTime::Start()
{
	is_stop_ = FALSE;
}