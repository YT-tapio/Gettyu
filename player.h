#pragma once
#include"DxLib.h"
#include"animation.h"



class Weapon;
class Input;

struct CapsuleData
{
	VECTOR start_pos;
	VECTOR end_pos;
	float vertical_num;
	float r;
	int div_num;
};

enum State
{
	Stand,
	Run,
	Jump
};

class Player
{
private:

	const float kWalkSpeed = 1.0f;
	const float kNormalSpeed = 2.0f;
	const float kDashSpeed = 2.8f;

	const float kGravity = 0.75f;		//重力
	const float kJumpPower = 3.5f;		//ジャンプ力


	//クラス関連
	Weapon* weapon_;
	Input* input_;

	State now_state_;

	MATRIX model_matrix_;				//

	Animation animation_;
	AnimationType now_type_;            //現在のプレイヤーのアニメ～しょん
	AnimationType before_type_;			//1つ前のアニメーション
	AnimationType before_before_type_;	//2つ前のアニメーション

	VECTOR pos_;						//ポジション
	VECTOR velocity_;
	VECTOR direction_;
	VECTOR rotation_;

	CapsuleData capsule_;

	float before_rot_;
	float target_rot_;


	bool is_ground_;					//地面の上にいるとき
	bool is_target_;					///ターゲットしているかどうか

	int model_;							//モデル

	int pad_input_num_;					//入力するパッドの番号

	//操作タイプ
	char key_input_[256] = {};
	XINPUT_STATE pad_input_ = {};

	float fall_speed_;

	float delta_time_;

	int frame_num_;


	//ターゲットの方向を見つける
	void MakeTargetRot(const VECTOR& target_pos, float& target_rot);

public:


	Player(VECTOR pos, int model, int pad_num, int div, float r, float vertical_num);

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


	void AttachWeapon(const TCHAR* frame_path, int model, float scale);


	void Update(const VECTOR& pos, const float& rotation);


	void InputMovement(const VECTOR& pos, float& rotation);


	void JumpAction(VECTOR& velocity);


	bool CheckGround();

	void CheckDirection(const VECTOR& pos, float& rotation);


	void CheckReverseRot(float& now_rot, float target_rot);


	void MakeLine(float& constant, const VECTOR& pos);


	void SetIsTarget(bool flag)
	{
		is_target_ = flag;
	}


	void TestFunc();

	MATRIX GetFrameMatrix();

	const VECTOR& GetPos() const { return pos_; }


	VECTOR GetCenterPos() { return { pos_.x,pos_.y + 15,pos_.z }; }


	const VECTOR GetDirection() const { return direction_; }

	const bool GetIsTarget() const { return is_target_; }


	const Input* GetInput() const { return input_; }

	const CapsuleData GetCapsuleData() const { return capsule_; }
};











//おふろ

