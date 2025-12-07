#pragma once
#include"DxLib.h"
#include"button_state.h"

class Button
{
private:

	const int kWhite		= GetColor(255, 255, 255);
	const int kRightGray			= GetColor(192, 192, 192);
	const int kGray		= GetColor(128, 128, 128);
	const float kSpeed		= 30.f;

	ButtonState state_;

	VECTOR pos_;

	int model_;

	int num_;				//自分の識別番号

	int debug_color_;


	float init_width_;		// 初期のサイズ	(横)
	float init_height_;		// 初期のサイズ	(縦)
	float width_;				// 今のサイズ		(横)
	float height_;			// 今のサイズ		(縦)
	
	
	float width_ratio_;
	float height_ratio_;

	bool is_select_;
	bool is_pussed_;

	bool* flag_;

	bool IsPushConditionMouse();

	bool IsPushConditionButton();

	bool IsReleaseCondition();

	/// @brief 自分が選ばれているのかの判断
	/// @param num 今選択されている識別番号
	void IsOnMouse(const int& num);

	void SelectUpdate();

	void PressedUpdate();

public:

	/// @brief 
	/// @param pos 中心位置 
	/// @param width よこ
	/// @param height たて
	/// @param path モデルのパス
	/// @param num_ 自分の識別番号
	Button(const VECTOR pos, const float width, const float height, const char* path, const int& num_,bool* flag);

	~Button();

	void Update(const int& num);

	void Draw();

	const ButtonState GetState() const { return state_; }

	const int GetNum() const { return num_; }
};