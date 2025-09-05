#pragma once
#include"DxLib.h"
#include"animation.h"
#include"super_attack.h"


class Weapon;
class Input;
class Stage;


struct CapsuleData
{
	VECTOR start_pos = { 0.f, 0.f, 0.f };
	VECTOR end_pos = { 0.f, 0.f, 0.f };
	float vertical_num = 0.f;
	float r = 0.f;
	int div_num = 0;
};

enum class State
{
	kStand,
	kSlowRun,
	kWalk,
	kRun,
	kJump,
	kFall,
	kAttack
};

class Player
{
private:

	const float kWalkSpeed = 1.0f;
	const float kNormalSpeed = 2.5f;
	const float kDashSpeed = 3.8f;
	const float kGravity = 0.75f;		//重力
	const float kJumpPower = 3.5f;		//ジャンプ力

	//クラス関連
	Weapon* weapon_;
	Input* input_;
	SuperAttack* super_attack_;

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

	//カメラのずらし量
	VECTOR camera_offset_dir;

	CapsuleData capsule_;

	float before_rot_;
	float target_rot_;

	

	bool is_ground_;					//地面の上にいるとき
	bool is_target_;					///ターゲットしているかどうか
	bool is_move_;

	bool is_super_attack_;
	bool is_switch_weapon_;

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
		super_attack_->SetDeltaTime(delta_time_);
	}


	void InputState();


	void AttachWeapon(const TCHAR* frame_path, int model, float scale);


	void Update(const VECTOR& pos, const float& rotation, Stage& stage);
	

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


	void OnHitRoof();


	void OnHitFloor();


	void TestFunc();

	void ResetFallSpeed() { fall_speed_ = 0.0f; }

	void SetIsGround(bool flag) { is_ground_ = flag; }

	void SetNowCameraSituation(int num) { super_attack_->SetNowSituatuin(num); }

	MATRIX GetFrameMatrix();

	VECTOR GetWeaponPos();

	int GetNowCameraSituationNum() { return super_attack_->GetNowSituation(); }

	const float GetFallSpeed()const { return fall_speed_; }

	const State& GetNowState() const { return now_state_; }

	const VECTOR& GetPos() const { return pos_; }

	const VECTOR OffsetCameraDirection() const { return camera_offset_dir; }

	VECTOR GetCenterPos() { return { pos_.x,pos_.y + 15.0f,pos_.z }; }


	const VECTOR GetRotation() const { return rotation_; }


	const VECTOR GetVelocity() const { return velocity_; }

	const VECTOR GetDirection() const { return direction_; }

	VECTOR GetSuperAttackEffectPosition() { return super_attack_->GetEffectPosition(); }

	const bool GetIsTarget() const { return is_target_; }

	const bool GetIsSwitchWeapon() const { return is_switch_weapon_; }

	const bool GetIsSuperAttack() const { return is_super_attack_; }

	const bool GetSuperAttackEffectIsPlay() const { return super_attack_->GetEffectIsPlay(); }

	const Input* GetInput() const { return input_; }

	const CapsuleData GetCapsuleData() const { return capsule_; }

	
};












