#include"camera.h"
#include"screen.h"
#include"EffekseerForDxLib.h"
#include"debug.h"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera()
{
	
}



void Camera::Init(const VECTOR& velocity)
{
	pos_ = VAdd(pos_, velocity);
}


void Camera::Awake(const VECTOR& pos, const VECTOR& target_pos, float fov)
{
	pos_			= VGet(0, 0, 0);
	target_pos_		= VGet(0, 0, 0);
	fov_			= fov;
	target_fov_		= 0.f;
	velocity_		= VGet(0, 0, 0);
	direction_		= VGet(0, 0, 0);

	//âúçs1.0Å`1000Ç‹Ç≈ÇÉJÉÅÉâÇÃï`âÊîÕàÕÇ∆Ç∑ÇÈ
	SetCameraNearFar(kNear, kFar);

	pos_ = pos;
	target_pos_ = target_pos;
	fov_ = fov;
	// éãñÏäpê›íË
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
