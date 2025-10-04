#pragma once
#include<iostream>

class Debug
{
private:

	//今のデバック数
	
	int current_num_ = 0;

	//コンストラクタを非公開
	Debug(){}

public:

	static Debug& GetInstance()
	{
		static Debug instance;	//性的変数としてインスタンスを定義
		return instance;
	}

	//コピーコンストラクタと代入演算子を消去
	Debug(const Debug&) = delete;
	Debug& operator = (const Debug&) = delete;

	/// <summary>
	/// プログラムの初めにリセット
	/// </summary>
	void Recet()
	{
		current_num_ = 0;
	}

	/// <summary>
	/// デバック表記をするたびに呼び出す
	/// </summary>
	void Add()
	{
		current_num_++;
	}

	/// <summary>
	/// デバックしてきた数を取得
	/// </summary>
	/// <returns></returns>
	const int GetCurrentNum() const { return current_num_; }

};