#pragma once


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

	ChangeType change_type_;

	VECTOR velocity_ = { 0,0,0 };
	VECTOR direction_ = { 0,0,0 };

	VECTOR pos_;
	VECTOR next_pos_;

	bool is_change_;

	//回転量
	float vertical_rad_ = 0.0f;
	float side_rad_ = 0.0f;

	float side_distance_ = 0.0f;
	float distance_ = 30.0f;

	float side_sensitivity_ = 1.0f;
	float vertical_sensitivity_ = 0.5f;
	float all_sensitivity_ = 5.5f;

	MousePoint now_mouse_pos_;
	MousePoint before_mouse_pos_;

	MousePoint dead_zone_;

	void MakeVertical();

	bool CheckMousePoint(MousePoint now_point, MousePoint before_point);

	/// <summary>
	/// 定まった角度の距離を受け取る
	/// </summary>
	VECTOR GetVelocityDecidedRad();

public:

	Brain();


	~Brain();

	
	void Update(const VECTOR& target_pos, const VECTOR& camera_pos, const Input* input);


	/// <summary>
	/// カメラが球体上に回る処理
	/// </summary>
	void SphereUpdate(const VECTOR& target_pos, const VECTOR& camera_pos,const Input* input);


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


	void Draw();
};