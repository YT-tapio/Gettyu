#pragma once
#include<iostream>
#include"DxLib.h"

class Debug
{
private:

	//今のデバック数
	
	int current_num_ = 0;
	//表示
	bool disp_ = FALSE;

	//コンストラクタを非公開
	Debug(){}

	
	void CheckChangeDisp()
	{

		if (CheckHitKey(KEY_INPUT_P))
		{
			disp_ = TRUE;
		}

		if (CheckHitKey(KEY_INPUT_O))
		{
			disp_ = FALSE;
		}

	}


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

	void Change()
	{
		CheckChangeDisp();
	}

	/// <summary>
	/// デバックしてきた数を取得
	/// </summary>
	/// <returns></returns>
	const int GetCurrentNum() const { return current_num_; }


	const bool GetDisp() const { return disp_; }

};