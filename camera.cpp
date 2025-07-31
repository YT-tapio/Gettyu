#include"camera.h"
#include"screen.h"
#include"EffekseerForDxLib.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(const VECTOR& pos,float fov)
	: pos_(VGet(0, 0, 0))
	, fov_(fov)	
	,target_fov_(0.0f)
	, velocity_({ 0,0,0 })
	, direction_({ 0,0,0 })
{
	//‰œs1.0`1000‚Ü‚Å‚ğƒJƒƒ‰‚Ì•`‰æ”ÍˆÍ‚Æ‚·‚é
	SetCameraNearFar(1.0f, 1000.0f);

	pos_ = pos;

	// ‹–ìŠpİ’è
	SetupCamera_Perspective(fov);
}

Camera::~Camera()
{

}


void Camera::Update(const VECTOR& target_pos,const VECTOR& velocity)
{
	VECTOR vel = velocity;

	pos_ = VAdd(pos_, vel);

	Effekseer_Sync3DSetting();
	
	SetLightPosition(pos_);
	
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos);
}


