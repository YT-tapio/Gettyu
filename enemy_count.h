#pragma once

class EnemyCount
{
private:

	int count_ = 0;


	EnemyCount(){}


public:


	static EnemyCount& GetInstance()
	{
		static EnemyCount instance;
		return instance;
	}

	//コピーコンストラクタと代入演算子を消去
	EnemyCount(const EnemyCount&) = delete;
	EnemyCount& operator = (const EnemyCount&) = delete;


	void SetCount(int count)
	{
		count_ = count;
	}


	const int GetCount() const
	{
		return count_;
	}


};
