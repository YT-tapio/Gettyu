#pragma once
#include"DxLib.h"
#include"animation.h"

class Player
{
private:

	const float kWalkSpeed = 1.0f;
	const float kNormalSpeed = 2.0f;
	const float kDashSpeed = 2.8f;
	
	const float kGravity = 0.3f;		//重力
	const float kJumpPower = 4.5f;		//ジャンプ力

	Animation animation_;
	AnimationType now_type_;            //現在のプレイヤーのアニメ～しょん
	AnimationType before_type_;			//1つ前のアニメーション
	AnimationType before_before_type_;	//2つ前のアニメーション

	VECTOR pos_;						//ポジション
	VECTOR velocity_;
	VECTOR direction_;
	VECTOR rotation_;

	bool is_ground_;					//地面の上にいるとき

	int model_;							//モデル

	
	int pad_input_num_;					//入力するパッドの番号

	//操作タイプ
	char key_input_[256] = {};
	XINPUT_STATE pad_input_ = {};

	float fall_speed_;


	//float speed_ = 0.5f;

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


	void Update(const VECTOR& pos,const float& rotation);

	
	void InputMovement(const VECTOR& pos, const float& rotation);


	void JumpAction(VECTOR& velocity);


	bool CheckGround();

	void CheckDirection(const VECTOR& pos, const float& rotation);


	void MakeLine(float& constant, const VECTOR& pos);


	const VECTOR& GetPos() const { return pos_; }
	

	VECTOR GetCenterPos() { return { pos_.x,pos_.y + 15,pos_.z }; }

};