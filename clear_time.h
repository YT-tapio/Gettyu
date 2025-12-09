#pragma once

class ClearTime
{
private:

	float clear_time_ = 0;

	ClearTime();

public:


	static ClearTime& GetInstance()
	{
		static ClearTime instance;
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	ClearTime(const ClearTime&) = delete;
	ClearTime& operator=(const ClearTime&) = delete;

	void SetClearTime(const float& time) { clear_time_ = time; }

	const float GetClearTime() const { return clear_time_; }

};