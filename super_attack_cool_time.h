#pragma once

class SuperAttackCoolTime
{
private:

	float ratio_ = 0.f;

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

	const float GetRatio() const { return ratio_; }

};