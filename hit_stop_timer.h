#pragma once

class HitStopTimer
{
private:

	const float kMin = 0.f;

	float timer_ = 0.f;

	HitStopTimer();

	

public:

	static HitStopTimer& GetInstance()
	{
		static HitStopTimer instance;
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	HitStopTimer(const HitStopTimer&) = delete;
	HitStopTimer& operator=(const HitStopTimer&) = delete;
	
	/// <summary>
	/// タイマーの更新
	/// </summary>
	void TimerUpdate();

	/// <summary>
	/// タイマーのセット
	/// </summary>
	/// <param name="time"></param>
	void SetTime(const float& time);

	/// <summary>
	/// ヒットストップ中なのかの判断
	/// </summary>
	/// <returns></returns>
	bool CheckHitStop();

};