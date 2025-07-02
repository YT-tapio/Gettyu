#pragma once
#include "DxLib.h"

enum ChangeType
{
	Turn,
	Straight
};

class Camera
{
private:

	const float  FovDegrees = 60.0f * DX_PI_F / 180.0f;		// カメラの視野角(度数)

	
	VECTOR pos_;	//ポジション
	VECTOR velocity_ = { 0,0,0 };
	VECTOR direction_ = { 0,0,0 };


	



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
	void Update(const VECTOR& target_pos, const VECTOR& velocity);



	const VECTOR& GetPos() const { return pos_; }

	
};
