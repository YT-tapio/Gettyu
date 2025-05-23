#pragma once
#include"DxLib.h"
#include"animation.h"

class Player
{
private:

	Animation animation_;
	AnimationType now_type_;            //現在のプレイヤーのアニメ～しょん
	AnimationType before_type_;			//1つ前のアニメーション
	AnimationType before_before_type_;	//2つ前のアニメーション

	VECTOR pos_;	//ポジション
	VECTOR velocity_;
	VECTOR direction_;
	int model_;			//モデル

	//入力するパッドの番号
	int pad_input_num_;

	//操作タイプ
	char key_input_[256] = {};
	XINPUT_STATE pad_input_ = {};

	float delta_time_;

public:

	
	Player(VECTOR pos, int model,int pad_num);

	~Player();


	void Init(VECTOR pos);


	void Draw();


	void AddAnim(const AnimationData& animation_data);


	void SetDeltaTime(float delta_time)
	{
		delta_time_ = delta_time;
		animation_.SetDeltaTime(delta_time);
	}


	void InputState();


	void Update();


	const VECTOR& GetPos() const { return pos_; }


};