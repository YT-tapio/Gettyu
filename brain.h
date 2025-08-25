#pragma once

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

	const int kMaxMouseDiff = 35.0f;

	const float kMaxMoveDistance = 0.0f;
	const float kCameraSpeed = 1.3f;

	const float kSuperAttackDist = 25.0f;
	const float kSuperAttackCameraMoveSpeed = 2.0f;


	ChangeType change_type_;

	VECTOR velocity_ = { 0,0,0 };
	VECTOR direction_ = { 0,0,0 };

	VECTOR pos_;
	VECTOR next_pos_;

	//みるぽじしょんのvelocity
	VECTOR target_velocity_;
	VECTOR next_target_pos_;	//次に見る場所

	bool is_change_;
	bool no_update_;

	//回転量
	float vertical_rad_ = 0.0f;
	float side_rad_ = 0.0f;

	float side_distance_ = 0.0f;
	float distance_ = 30.0f;

	float side_sensitivity_ = 1.0f;
	float vertical_sensitivity_ = 0.5f;
	float all_sensitivity_ = 5.5f;

	float delta_time_ = 0.0f;

	MousePoint now_mouse_pos_;
	MousePoint before_mouse_pos_;

	MousePoint dead_zone_;

	

	void MakeVertical();

	VECTOR OffsetPassingVel(const VECTOR& now_pos, const VECTOR& target_pos, const float& speed);

	bool CheckMousePoint(MousePoint now_point, MousePoint before_point);

	bool CheckSamePos(const VECTOR& pos1, const VECTOR& pos2)
	{
		if (pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z)
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

	void Update(const VECTOR& target_pos, const VECTOR& camera_pos, std::shared_ptr<Player> player);


	/// <summary>
	/// カメラが球体上に回る処理
	/// </summary>
	void SphereUpdate(const VECTOR& target_pos, const VECTOR& camera_pos,const Input* input);


	void SuperAttackUpdate(const VECTOR& camera_pos, const VECTOR& now_target_pos, std::shared_ptr<Player> player);

	void ChangeCamera();

	
	void SetRad(const VECTOR& target_pos, const VECTOR& player_pos);


	void SetVelocity(const VECTOR& target_pos,const VECTOR& camera_pos);



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