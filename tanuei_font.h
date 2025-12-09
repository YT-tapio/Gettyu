#pragma once

class TanueiFont
{
private:

	int handle_;

	TanueiFont();

public:

	static TanueiFont& GetInstance()
	{
		static TanueiFont instance;
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	TanueiFont(const TanueiFont&) = delete;
	TanueiFont& operator=(const TanueiFont&) = delete;

	void Awake();

};