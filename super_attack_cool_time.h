#pragma once
#include"DxLib.h"
class SuperAttackCoolTime
{
private:

	float ratio_ = 0.f;
	bool is_active_ = FALSE;
	SuperAttackCoolTime();

public:

	static SuperAttackCoolTime& GetInstance()
	{
		static SuperAttackCoolTime instance;
		return instance;
	}

	//コピーコンストラクタと代入演算子を消去
	SuperAttackCoolTime(const SuperAttackCoolTime&) = delete;
	SuperAttackCoolTime& operator = (const SuperAttackCoolTime&) = delete;

	void SetRatio(const float& ratio);

	void SetIsActive(const bool& flag);

	const float GetRatio() const { return ratio_; }

	const bool GetIsActive() const { return is_active_; }
};