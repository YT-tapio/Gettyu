#include"camera.h"

Camera::Camera()
{
	//‰œs1.0`1000‚Ü‚Å‚ğƒJƒƒ‰‚Ì•`‰æ”ÍˆÍ‚Æ‚·‚é
	SetCameraNearFar(1.0f, 1000.0f);

	pos_ = VGet(10, 300, -300);
}

Camera::~Camera()
{

}


void Camera::Update(const VECTOR& target_pos)
{
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos);
}