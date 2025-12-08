#include"camera.h"
#include"screen.h"
#include"EffekseerForDxLib.h"
#include"debug.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include"vector_assistant.h"

Camera::Camera()
{
	
}



void Camera::Init(const VECTOR& velocity)
{
	pos_ = VAdd(pos_, velocity);
}


void Camera::Awake(const VECTOR& pos, const VECTOR& target_pos, float fov)
{
	pos_					= VectorAssistant::GetZeroVec();
	target_pos_		= VectorAssistant::GetZeroVec();
	fov_					= fov;
	target_fov_		= 0.f;
	velocity_			= VectorAssistant::GetZeroVec();
	direction_			= VectorAssistant::GetZeroVec();

	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);

	pos_ = pos;
	target_pos_ = target_pos;
	fov_ = fov;
	// 視野角設定
	SetupCamera_Perspective(fov_);
}


void Camera::Update(const VECTOR& velocity, const VECTOR& target_velocity)
{
	VECTOR vel = velocity;
	VECTOR target_vel = target_velocity;
	pos_ = VAdd(pos_, vel);
	target_pos_ = VAdd(target_pos_, target_vel);
	Effekseer_Sync3DSetting();
	
	SetLightPosition(pos_);
	//ターゲットの方向へのライトを出す
	VECTOR light_dir = VectorAssistant::GetDir(pos_, target_pos_);
	SetLightDirection(light_dir);
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos_);
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(fov_);

}

void Camera::Draw()
{
	if (Debug::GetInstance().GetDisp())
	{
		DrawFormatString(100, 120, GetColor(255, 255, 255), "x:%.2f,y:%.2f,z:%.2f", pos_.x, pos_.y, pos_.z);

		DrawSphere3D(target_pos_, 1, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}
}

void Camera::OriginalSetting()
{
	const VECTOR kInitPos = VectorAssistant::GetZeroVec();
	const VECTOR kInitTargetPos	= VGet(0.f, 0.f, 10.f);
	const VECTOR kInitLightDir		= VGet(0.f, 0.f, 1.f);
	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);
	//ポジションの指定
	SetCameraPositionAndTarget_UpVecY(kInitPos, kInitTargetPos);
	// 視野角設定
	SetupCamera_Perspective(fov_);
}

void Camera::BeforeSetting()
{
	SetLightPosition(pos_);
	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);
	//ポジションの指定
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos_);
	// 視野角設定
	SetupCamera_Perspective(fov_);
}
