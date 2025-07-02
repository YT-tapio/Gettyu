#include"camera.h"
#include"screen.h"
#include"EffekseerForDxLib.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera()
{
	//‰œs1.0`1000‚Ü‚Å‚ğƒJƒƒ‰‚Ì•`‰æ”ÍˆÍ‚Æ‚·‚é
	SetCameraNearFar(1.0f, 1000.0f);

	pos_ = VGet(0, 0, -0);

	// ‹–ìŠpİ’è
	SetupCamera_Perspective(FovDegrees);
}

Camera::~Camera()
{

}


void Camera::Update(const VECTOR& target_pos,const VECTOR& velocity)
{
	pos_ = VAdd(target_pos, velocity);

	Effekseer_Sync3DSetting();
	
	SetLightPosition(pos_);
	
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos);
}


