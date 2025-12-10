#pragma once

class EnemyGetNum
{
private:

	int num_ = 0;

	EnemyGetNum();

public:

	static EnemyGetNum& GetInstance()
	{
		static EnemyGetNum instance; 
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	EnemyGetNum(const EnemyGetNum&) = delete;
	EnemyGetNum& operator=(const EnemyGetNum&) = delete;

	void Reset();


	void AddNum();

	const int GetNum() const { return num_; }

};