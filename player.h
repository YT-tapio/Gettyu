#pragma once
#include"DxLib.h"
#include"animation.h"
#include"super_attack.h"
#include"enemy_base.h"
#include"vibration.h"
#include"sound_vibration.h"


struct MixamoBonePath;
class CharacterBase;
class WeaponBase;
class Input;
class Stage;
class Bat;
class WarpRod;
class WizardStaff;
class EnemyBase;
class SoundVibration;
class CollisionBase;
enum class WeaponName;

struct CapsuleData
{
	VECTOR start_pos = { 0.f, 0.f, 0.f };
	VECTOR end_pos = { 0.f, 0.f, 0.f };
	float vertical_num = 0.f;
	float r = 0.f;
	int div_num = 0;
};

enum class PlayerState
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

	const char* kGameClearEffectPath		= "data/effect/Pierre02/FeatherBomb.efkefc";

	const float kWalkSpeed					= 1.0f;
	const float kNormalSpeed				= 2.5f;
	const float kDashSpeed					= 5.5f;
	const float kGravity							= 0.75f;			// 重力
	const float kJumpPower					= 3.5f;				// ジャンプ力

	const float kFastRunSound				= 2.f;				// 大きい音
	const float kNormalRunSound			= 1.f;				// 歩いているのが普通
	const float kWalkSound					= 0.1f;				// ちょっとだけ聞かれているような
	
	const float kVacuumSound				= 2.f;				// 

	const float kAttackAnimTimeMin		= 17.5f;
	const float kAttackAnimTimeMax		= 21.f;

	const float kDanceStop					= 106.4f;

	const float kGameClearEffectHideTime = 30.f;

	const VECTOR kScale = VGet(0.01f, 0.01f, 0.01f);

	//MixamoBonePath bone_;

	//今何の武器を持っているかを持たせておく
	WeaponName now_weapon_name_;

	//クラス関連
	WeaponBase* weapon_;
	SuperAttack* super_attack_;

	Effect* super_weapon_spin_effect_ = new Effect("data/effect/NextSoft01/MagicTornade.efkefc", VGet(0.f,0.f,0.f), VGet(0.f, 0.f, 0.f), 7.f, 7.f, 150.f, TRUE);
	std::shared_ptr<Effect> game_clear_effect_;

	SoundVibration* sound_vibration_ = new SoundVibration();


	PlayerState now_state_;

	MATRIX model_matrix_;				//

	std::shared_ptr<Animation> animation_;
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

	std::shared_ptr<CollisionBase> coll_;
	std::shared_ptr<CollisionBase> gravity_check_coll_;
	//const VibrationData kVacuumVibration = { 500,700 };


	float before_rot_;
	float target_rot_;


	bool is_ground_;					//地面の上にいるとき
	bool is_target_;					///ターゲットしているかどうか
	bool is_move_;
	bool is_camera_blend_;			//brainのis_blend_の情報を受け取る
	bool is_camera_target_blend_;	//brainのis_target_blend_の情報を受け取る

	bool is_attack_;
	bool is_super_attack_;
	bool is_switch_weapon_;

	bool is_vacuum_ = FALSE;

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

	bool SuperAttackCondition();

	void CheckIsGround(Stage& stage);

	void GameClearUpdate(const VECTOR& camera_pos);

	void DecideAnimation();

public:


	Player(VECTOR pos, int model, int pad_num, int div, float r, float vertical_num);

	~Player();

	void Init(VECTOR pos);


	void Draw();


	void Debug();


	void AddAnim();


	void SetDeltaTime(const float& delta_time);


	void AttachWeapon(WeaponName name);


	void Update(Stage& stage, float target_rot);
	

	void InputMovement(const VECTOR& pos, float& rotation);


	void JumpAction(VECTOR& velocity);


	bool CheckGround();

	void CheckDirection(const VECTOR& pos, float& rotation);


	void CheckReverseRot(float& now_rot, float target_rot);


	void MakeLine(float& constant, const VECTOR& pos);


	//敵を捕まえたかどうかの処理を行う
	void IsHitEnemy(EnemyBase* enemy, bool& got);


	void Vibration(const VibrationData& data);


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

	void SetIsSuperAttack(bool flag) { is_super_attack_ = flag; }

	void SetIsBlend(bool flag) { is_camera_blend_ = flag; }

    void SetIsTargetBlend(bool flag) { is_camera_target_blend_ = flag; }

	MATRIX GetFrameMatrix();

	VECTOR GetWeaponPos();

	int GetNowCameraSituationNum() { return super_attack_->GetNowSituation(); }

	const float GetFallSpeed()const { return fall_speed_; }

	const float GetSuperAttackEffectPlayCount() const { return super_attack_->GetEffectPlayCount(); }

	const PlayerState& GetNowState() const { return now_state_; }

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

	const bool GetIsVacuum() const { return is_vacuum_; }


	const CapsuleData GetCapsuleData() const { return capsule_; }

	const float GetSoundVibrationNum() const { return sound_vibration_->GetNum(); }
};












