#pragma once

class Timer
{
public:

	float time_;
	Timer();
public:


	static Timer& GetInstance()
	{
		static Timer instance;
		return instance;
	}

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	//‚±‚±‚Å‰ÁZ‚µ‚Ä‚¢‚­
	void Update();


	//•\¦‚ğs‚¤
	void Debug();


	float GetTime();

};
