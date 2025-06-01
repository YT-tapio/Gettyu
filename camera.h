#pragma once
#include "DxLib.h"

struct MousePoint
{
	int x;
	int y;
};

class Camera
{
private:

	VECTOR pos_;	//ポジション
	VECTOR velocity_ = { 0,0,0 };
	VECTOR direction_ = { 0,0,0 };

	float vertical_rad_ = 0.0f;
	float side_rad_ = 0.0f;

	float side_distance_ = 0.0f;
	float distance_ = 30.0f;

	float sensitivity_ = 2.5f;

	MousePoint now_mouse_pos_;
	MousePoint before_mouse_pos_;

	MousePoint dead_zone_;



public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const VECTOR& target_pos);


	void MakeVertical(const VECTOR& pos);


	bool CheckMousePoint(MousePoint now_point, MousePoint before_point);


	const VECTOR& GetPos() const { return pos_; }


	
};
