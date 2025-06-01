#include"camera.h"
#include"screen.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera()
{
	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 1000.0f);

	pos_ = VGet(10, 300, -300);
}

Camera::~Camera()
{

}


void Camera::Update(const VECTOR& target_pos)
{
	GetMousePoint(&now_mouse_pos_.x, &now_mouse_pos_.y);



	if (CheckMousePoint(now_mouse_pos_, before_mouse_pos_))
	{
		if (side_rad_ > (M_PI * 2)) { side_rad_ = 0; }

		MakeVertical(target_pos);

		if (now_mouse_pos_.x > before_mouse_pos_.x)
		{
			float constant = now_mouse_pos_.x - before_mouse_pos_.x;
			side_rad_ += static_cast<float>((M_PI / 180) * (constant / 10) * sensitivity_);
		}

		if (now_mouse_pos_.x < before_mouse_pos_.x)
		{
			float constant = before_mouse_pos_.x - now_mouse_pos_.x;
			side_rad_ -= static_cast<float>((M_PI / 180) * (constant / 10) * sensitivity_);
		}


		//pos_.y = 40;

		int MouseX = (float(kGameWidth) * 0.5f), MouseY = (float(kGameHeight) * 0.5f);
		SetMousePoint(MouseX, MouseY);
		GetMousePoint(&now_mouse_pos_.x, &now_mouse_pos_.y);
		before_mouse_pos_ = now_mouse_pos_;


	}

	direction_.x = sinf(side_rad_);
	direction_.z = cosf(side_rad_);

	direction_ = VNorm(direction_);

	//velocity_ = VScale(direction_, distance_);

	velocity_.x = direction_.x * side_distance_;
	velocity_.z = direction_.z * side_distance_;

	pos_ = VAdd(target_pos, velocity_);


	
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos);
}

void Camera::MakeVertical(const VECTOR& pos)
{
	if (vertical_rad_ > (M_PI * 2)) { vertical_rad_ = 0; }


	//上に行くとき
	if (now_mouse_pos_.y > before_mouse_pos_.y)
	{
		float constant = now_mouse_pos_.y - before_mouse_pos_.y;
		vertical_rad_ += static_cast<float>((M_PI / 180) * (constant / 10) * sensitivity_);
	}

	//下に行くとき
	if (now_mouse_pos_.y < before_mouse_pos_.y)
	{
		float constant = before_mouse_pos_.y - now_mouse_pos_.y;
		vertical_rad_ -= static_cast<float>((M_PI / 180) * (constant / 10) * sensitivity_);
	}

	//真上に来た時に後ろに行かないように
	if (vertical_rad_ > static_cast<float>((M_PI / 180) * 80))
	{
		vertical_rad_ = static_cast<float>((M_PI / 180) * 80);
	}

	//真下に来た時に後ろに行かないように
	if (vertical_rad_ < -(static_cast<float>((M_PI / 180) * 80)))
	{
		vertical_rad_ = -(static_cast<float>((M_PI / 180) * 80));
	}

	/*----横の長さをだす(cos)---*/

	velocity_.y = (sinf(vertical_rad_)) * distance_;
	side_distance_ = (cosf(vertical_rad_)) * distance_;

}


bool Camera::CheckMousePoint(MousePoint now_point, MousePoint before_point)
{
	if (now_point.x < (before_point.x - dead_zone_.x) ||
		now_point.x >(before_point.x + dead_zone_.x) ||
		now_point.y < (before_point.y - dead_zone_.y) ||
		now_point.y >(before_point.y + dead_zone_.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
