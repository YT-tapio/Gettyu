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


	float GetTimeRatio();

	void Debug();
};