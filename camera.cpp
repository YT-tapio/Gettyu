#include"camera.h"
#include"screen.h"
#include"EffekseerForDxLib.h"
#include"debug.h"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(const VECTOR& pos, const VECTOR& target_pos, float fov)
	: pos_(VGet(0, 0, 0))
	, target_pos_(VGet(0,0,0))
	, fov_(fov)	
	,target_fov_(0.0f)
	, velocity_({ 0,0,0 })
	, direction_({ 0,0,0 })
{
	//âúçs1.0Å`1000Ç‹Ç≈ÇÉJÉÅÉâÇÃï`âÊîÕàÕÇ∆Ç∑ÇÈ
	SetCameraNearFar(1.0f, 1000.0f);

	pos_ = pos;
	target_pos_ = target_pos;
	fov_ = fov;
	// éãñÏäpê›íË
	SetupCamera_Perspective(fov_);
}

Camera::~Camera()
{

}


void Camera::Init(const VECTOR& velocity)
{
	pos_ = VAdd(pos_, velocity);
}


void Camera::Update(const VECTOR& velocity, const VECTOR& target_velocity)
{
	VECTOR vel = velocity;
	VECTOR target_vel = target_velocity;
	pos_ = VAdd(pos_, vel);
	target_pos_ = VAdd(target_pos_, target_vel);
	Effekseer_Sync3DSetting();
	
	//SetLightPosition(pos_);
	
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos_);
	SetCameraNearFar(1.0f, 1000.0f);
	SetupCamera_Perspective(fov_);
	if (CheckHitKey(KEY_INPUT_T))
	{
		// éãñÏäpê›íË
		SetupCamera_Perspective((DX_PI_F / 180.0f) * 75.0f);
	}
	

}

void Camera::Draw()
{
	if (Debug::GetInstance().GetDisp())
	{
		DrawFormatString(100, 120, GetColor(255, 255, 255), "x:%.2f,y:%.2f,z:%.2f", pos_.x, pos_.y, pos_.z);

		DrawSphere3D(target_pos_, 1, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}

	
}


