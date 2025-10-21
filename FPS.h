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
	float time_scale_;

	int count_;

	FPS();

public:

	
	static FPS& GetInstance()
	{
		static FPS instance;
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	FPS(const FPS&) = delete;
	FPS& operator=(const FPS&) = delete;

	void Init();

	
	void Update();


	void Wait();


	void SetTimeScale(float time_scale)
	{
		time_scale_ = time_scale;
	}


	void SetPrevTime()
	{
		prev_time_ = now_time_;
	}

	
	const float GetDeltaTime() const { return delta_time_; }


	void DrawDeltaTime();


	void DrawTimeScale();

};
