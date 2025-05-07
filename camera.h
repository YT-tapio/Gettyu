#pragma once
#include "DxLib.h"

class Camera
{
private:

	VECTOR pos_;	//ポジション

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


	const VECTOR& GetPos() const { return pos_; }

};
