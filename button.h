#pragma once
#include"DxLib.h"

class Button
{
private:

	VECTOR pos_;
	int width_;
	int height_;

	int model_;

	bool is_select_;
	bool is_push_;

	int num_;


	bool IsPushCondition();

	bool IsReleaseCondition();

	/// @brief 自分が選ばれているのかの判断
	/// @param num 今選択されている識別番号
	bool IsOnMouse(const int& num);

	bool IsPush();

	void IsPushUpdate();

public:

	/// @brief 
	/// @param pos 中心位置 
	/// @param width よこ
	/// @param height たて
	/// @param path モデルのパス
	/// @param num_ 自分の識別番号
	Button(const VECTOR pos, const int width, const int height, const char* path, const int& num_);

	~Button();

	void Update(const int& num);

	void Draw();

};