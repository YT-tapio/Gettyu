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

	const float kNear	= 1.f;
	const float kFar	= 1000.f;
	float fov_;		// 今のカメラの視野角(度数)
	float target_fov_;	// 次のカメラの視野角(度数)
	
	VECTOR pos_;				//ポジション
	VECTOR target_pos_;			//見る場所
	VECTOR velocity_;
	VECTOR direction_;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

public:

	static Camera& GetInstance()
	{
		static Camera instance;	//性的変数としてインスタンスを定義
		return instance;
	}

	//コピーコンストラクタと代入演算子を消去
	Camera(const Camera&) = delete;
	Camera& operator = (const Camera&) = delete;


	//
	void Init(const VECTOR& velocity);

	void Awake(const VECTOR& pos, const VECTOR& target_pos, float fov);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const VECTOR& velocity, const VECTOR& target_velocity);

	
	void Draw();

	void SetFov(float fov) { fov_ = fov; }

	const VECTOR& GetPos() const { return pos_; }

	const VECTOR& GetTargetPos()const { return target_pos_; }

	const float GetFov() const { return fov_; }

	const float GetNear() const { return kNear; }

	const float GetFar() const { return kFar; }

	

};
