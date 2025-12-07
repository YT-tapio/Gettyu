#pragma once

class ConditionTimer
{
private:

	float max_time_;
	float timer_;

	bool is_stop_;

public:


	ConditionTimer(float max_time);

	~ConditionTimer();


	void Update();

	void Reset();

	void Start();

	void Stop();

	float GetTimeRatio();

	const float GetNowTimer()const;

	bool GetIsEnd();

	void Debug();
};