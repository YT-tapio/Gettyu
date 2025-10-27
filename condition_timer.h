#pragma once

class ConditionTimer
{
private:

	float max_time_;
	float timer_;

public:


	ConditionTimer(float max_time);

	~ConditionTimer();


	void Update();

	void Reset();

	float GetTimeRatio();

	void Debug();
};