#pragma once
#define DEF_FPSCONTROLL_H
#include"DxLib.h"

class FPS
{

private:

	const float  kTargetFps = 60.0f;

	LONGLONG prev_time_;
	LONGLONG now_time_;

	float delta_time_;
	float now_fps_;

	int count_;

public:

	FPS()
	{
		Init();
	}
	

	void Init();

	
	void Update();


	void Wait();


	void SetPrevTime()
	{
		prev_time_ = now_time_;
	}


	const float GetDeltaTime() const { return delta_time_; }


	void Draw();

};
