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
	
	VECTOR pos_;				//ポジション
	VECTOR target_pos_;		//見る場所
	VECTOR velocity_;
	VECTOR direction_;


	



public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera(const VECTOR& pos, const VECTOR& target_pos, float fov);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();


	//
	void Init(const VECTOR& velocity);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const VECTOR& velocity, const VECTOR& target_velocity);

	
	void Draw();


	const VECTOR& GetPos() const { return pos_; }


	const VECTOR& GetTargetPos()const { return target_pos_; }

	void SetFov(float fov) { fov_ = fov; }

	
};
