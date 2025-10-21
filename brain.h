#pragma once
#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include"virtual_camera.h"
#include"super_attack_camera.h"
#include"sphere_camera.h"
#include"get_camera.h"
#include"vacuum_camera.h"
class Player;
class Input;



struct MousePoint
{
	int x;
	int y;
};


class Brain
{
private:

	
	

	const float kMaxMouseDiff = 35.0f;

	const float kMaxMoveDistance = 0.0f;
	const float kCameraSpeed = 1.3f;

	const float kSuperAttackZeroDist = 25.0f;
	const float kSuperAttackCameraMoveSpeed = 2.0f;

	const float kSuperAttackFirstDist = 50.0f;
	const float kSuperAttackFirstCameraMoveSpeed = 2.0f;
	const float kSuperAttackFirstSideRad = 0;
	
	//カメラの見る位置をoffsetするときのスピード
	const float kSuperAttackCameraTargetPosSpeed = 1.3f;

	//vacuumの定数
	const float kVacuumDist = 80.f;			//vacuumの時の距離
	const float kVacuumVerticalRad = (1 * static_cast<float>((M_PI / 180) * 45));

	//球体上に動くカメラ
	BaseVirtualCamera* sphere_camera_;

	//敵をゲットしたときのかめら
	BaseVirtualCamera* get_camera_;

	//必殺技のカメラ
	BaseVirtualCamera* super_attack_camera_[3];

	//ついてくるカメラ
	BaseVirtualCamera* tracking_camera_;

	//吸い込んでいるときのカメラ
	BaseVirtualCamera* vacuum_camera_;

	ChangeType change_type_;

	VECTOR velocity_ = { 0,0,0 };
	VECTOR direction_ = { 0,0,0 };

	VECTOR pos_;
	VECTOR start_pos_;		// ブレンドするときの最初のpos
	VECTOR next_pos_;		// ブレンドするときの次のpos

	//みるぽじしょんのvelocity
	VECTOR target_velocity_;
	VECTOR start_target_pos_;	//
	VECTOR next_target_pos_;	//次に見る場所

	//ゲットしたときにターゲットとなるpos
	VECTOR get_camera_target_pos_;

	//int群
	//現在のvirtualcameraの名前を保存
	int camera_name_;

	int vibration_count_;
	int super_attack_vibration_power_;

	bool is_change_;
	bool no_update_;
	bool is_blend_;			// 座標のブレンド
	bool is_target_blend_;	// 見る座標のブレンド

	bool is_init = FALSE;

	bool is_blend_tracking_ = FALSE;//かめらを切り替えたブレンド中でもちゃんと動く奴はこのフラグをTRUEに

	//回転量
	float vertical_rad_ = 0.0f;
	float side_rad_ = 0.0f;
	
	float camera_to_enemy_height_ = 0.f;

	float side_distance_ = 0.0f;
	float distance_ = 40.0f;

	float side_sensitivity_ = 1.0f;
	float vertical_sensitivity_ = 0.5f;
	float all_sensitivity_ = 5.5f;

	float delta_time_ = 0.0f;

	float get_camera_vertical_rad_ = 0.f;
	float get_camera_side_rad_ = 0.f;


	float camera_to_enemy_dist_ = 0.f;

	float blend_speed_ = 0.f;
	float target_blend_speed_ = 10.f;

	float get_dist_ = 0.f;
	float offset_line_timer_ = 0.f;

	float vacuum_offset_dist_ = 0.f;

	MousePoint now_mouse_pos_;
	MousePoint before_mouse_pos_;

	MousePoint dead_zone_;

	

	void MakeVertical();


	//受け取った引数のポジションから指定したdist分のradの位置を返す
	VECTOR GetRotatedByTheDistanceFromThePos(const float ver_rad, const float side_rad, const float dist,const VECTOR& center_pos);

	VECTOR OffsetPassingVel(const VECTOR& now_pos, const VECTOR& target_pos, const VECTOR& velocity,bool& flag);

	//superattackの時かめら位置更新一回目
	VECTOR SetSuperAttackFrontPos(std::shared_ptr<Player> player);


	/// <summary>
	/// 引数のposからdistance分離れた距離を受け取る
	/// </summary>
	/// <param name="pos">中心の位置</param>
	/// <param name="distance">どれだけ離れているのか</param>
	/// <param name="ver_rad">たての角度</param>
	/// <param name="side_rad">よこの角度</param>
	/// <returns></returns>
	VECTOR GetThisDistanceOfffsetPos(const VECTOR& pos, const float& distance, const float& ver_rad, 
		const float& side_rad,std::shared_ptr<Player> player);


	/// <summary>
	/// エフェクトの後ろの位置
	/// </summary>
	/// <param name="pos">エフェクトの位置</param>
	/// <returns></returns>
	VECTOR GetSuperAttackEffectBehindPos(std::shared_ptr<Player> player);

	/// <summary>
	/// weaponの位置からのoffset値
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	VECTOR GetSuperAttackWeaponCameraPos(const VECTOR& pos);


	bool CheckMousePoint(MousePoint now_point, MousePoint before_point);



	bool CheckSamePos(const VECTOR& pos1, const VECTOR& pos2)
	{
		//これでいいわけがない(同じにならない可能性が大いにあるので許容範囲を決める
		const float kOffset = 10.0;

		//pos1とpos2の距離を見る
		VECTOR distance = VSub(pos1, pos2);
		
		float size = VSize(distance);

		//各座標がkOffsetの中なら一致とする
		if (-kOffset <= size && size <= kOffset)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}




	/// <summary>
	/// 定まった角度の距離を受け取る
	/// </summary>
	VECTOR GetVelocityDecidedRad();

public:

	Brain(const VECTOR& next_target_pos);


	~Brain();


	void Init(const VECTOR& camera_pos, const VECTOR& player_pos);

	/*----------virttual_cameraのInit-----------*/



	void GetInit(const VECTOR& camera_pos, const VECTOR& enemy_pos);

	//ついびのinit
	void TrackingInit(const VECTOR& center_pos);


	void Update(const VECTOR& target_pos, const VECTOR& camera_pos, std::shared_ptr<Player> player);


	//
	void ChangeCameraInit(int& before_camera_name, const VECTOR& camera_pos, std::shared_ptr<Player>player, const VECTOR& now_target_pos, bool& is_init);

	//
	void VirtualCameraUpdate(std::shared_ptr<Player>player, const VECTOR& camera_pos, const VECTOR& now_target_pos);

	/// <summary>
	/// カメラが球体上に回る処理
	/// </summary>
	void SphereUpdate(const VECTOR& target_pos, const VECTOR& camera_pos,const Input* input);

	//追尾のアップデート
	void TrackingUpdate(const VECTOR& now_camera_pos, std::shared_ptr<Player> player);

	//吸引時のカメラのアプデ
	void VacuumUpdate(std::shared_ptr<Player>player, const VECTOR& camera_pos);

	void SuperAttackUpdate(const VECTOR& camera_pos, const VECTOR& now_target_pos, std::shared_ptr<Player> player);

	/// <summary>
	/// ゲットしたときのカメラの更新処理
	/// </summary>
	void GetCameraUpdate(const VECTOR& pos,const VECTOR& camera_pos, const VECTOR& target_pos);


	void ChangeCamera();

	
	void Vibration();


	void VacuumVibration();

	void SetRad(const VECTOR& target_pos, const VECTOR& player_pos);


	void SetVelocity(const VECTOR& target_pos,const VECTOR& camera_pos);

	void SetDeltaTime(const float& delta_time) { delta_time_ = delta_time; }


	/// <summary>
	/// カメラを切り替えた時のだんだんとよるやつ
	/// </summary>
	/// <param name="this_pos">よると決めた時の元の座標</param>
	/// <param name="now_camera_pos">今のカメラのポジション</param>
	/// <param name="next_pos">次のポジション</param>
	/// <param name="time">何秒でよるのか(指定したい時間*delta_time_)</param>
	/// <returns>移動量</returns>
	VECTOR GetStartToNextVelocity(const VECTOR& start_pos, const VECTOR& now_camera_pos, const VECTOR& next_pos,const float& time,bool& flag);

	/// <summary>
	/// 現在の位置からターゲットの距離までの距離をだす
	/// </summary>
	VECTOR GetFutureToNowPositionVelocity(const VECTOR& future_pos, const VECTOR& now_pos);

	/// <summary>
	/// 移動量が既定の量を超えているときvelocityの値を調整する
	/// </summary>
	VECTOR OffsetVelocity(const VECTOR& velocity,float offset_num);

	/// <summary>
	/// ターゲットを中心に指定された横と縦のradのポジションを調べる
	/// </summary>
	VECTOR GetPositionFromTarget(const VECTOR& target_pos);


	const bool GetIsChange() const { return is_change_; }

	/// <summary>
	/// 横の回転量を取得する
	/// </summary>
	const float GetSideRad() const { return side_rad_; }


	const VECTOR GetVelocity() const { return velocity_; }


	const VECTOR GetTargetVelocity() const { return target_velocity_; }


	void Draw();
};