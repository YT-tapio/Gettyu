#include"camera.h"
#include"screen.h"
#include"EffekseerForDxLib.h"

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
	//‰œs1.0`1000‚Ü‚Å‚ğƒJƒƒ‰‚Ì•`‰æ”ÍˆÍ‚Æ‚·‚é
	SetCameraNearFar(1.0f, 1000.0f);

	pos_ = pos;
	target_pos_ = target_pos;

	// ‹–ìŠpİ’è
	SetupCamera_Perspective(fov);
}

Camera::~Camera()
{

}


void Camera::Init(const VECTOR& velocity)
{
	//pos_ = VGet(pos_,velocity)
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

	if (CheckHitKey(KEY_INPUT_T))
	{
		// ‹–ìŠpİ’è
		SetupCamera_Perspective((DX_PI_F / 180.0f) * 75.0f);
	}
	

}


