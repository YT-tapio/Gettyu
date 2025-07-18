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

	float fov_;		// 今のカメラの視野角(度数)
	float target_fov_;	// 次のカメラの視野角(度数)
	
	VECTOR pos_;	//ポジション
	VECTOR velocity_;
	VECTOR direction_;


	



public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera(const VECTOR& pos, float fov);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const VECTOR& target_pos, const VECTOR& velocity);



	const VECTOR& GetPos() const { return pos_; }


	void SetFov(float fov) { fov_ = fov; }

	
};
