#include<iostream>
#include"DxLib.h"
#include"camera.h"
#include"screen.h"
#include"brain.h"

#define _USE_MATH_DEFINES
#include <math.h>

Brain::Brain()
	:pos_(VGet(0,0,0))
	,next_pos_(VGet(0,0,0))
	,is_change_(FALSE)
	,change_type_(ChangeType::Straight)
{

}


Brain::~Brain()
{

}

/*----------------private-----------------*/

void Brain::MakeVertical(const VECTOR& pos)
{
	if (vertical_rad_ > (M_PI * 2)) { vertical_rad_ = 0; }


	//上に行くとき
	if (now_mouse_pos_.y > before_mouse_pos_.y)
	{
		float constant = now_mouse_pos_.y - before_mouse_pos_.y;

		if (constant > kMaxMouseDiff)
		{
			constant = kMaxMouseDiff;
		}

		vertical_rad_ += static_cast<float>((M_PI / 180) * (constant / 10) * sensitivity_);
	}

	//下に行くとき
	if (now_mouse_pos_.y < before_mouse_pos_.y)
	{
		float constant = before_mouse_pos_.y - now_mouse_pos_.y;

		if (constant > kMaxMouseDiff)
		{
			constant = kMaxMouseDiff;
		}

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

bool Brain::CheckMousePoint(MousePoint now_point, MousePoint before_point)
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



/*---------------public---------------*/

void Brain::Update(const VECTOR& target_pos)
{

	SphereUpdate(target_pos);

}


void Brain::SphereUpdate(const VECTOR& target_pos)
{
	//マウスポインターの取得
	GetMousePoint(&now_mouse_pos_.x, &now_mouse_pos_.y);


	//前回と現在のポインターの位置が違うとき
	if (CheckMousePoint(now_mouse_pos_, before_mouse_pos_))
	{
		//横の回転が360を超えないように
		if (side_rad_ > (M_PI * 2)) { side_rad_ = 0; }

		//縦の回転を作る
		MakeVertical(target_pos);

		//
		if (now_mouse_pos_.x > before_mouse_pos_.x)
		{
			float constant = now_mouse_pos_.x - before_mouse_pos_.x;

			if (constant > kMaxMouseDiff)
			{
				constant = kMaxMouseDiff;
			}

			side_rad_ += static_cast<float>((M_PI / 180) * (constant / 10) * sensitivity_);
		}

		if (now_mouse_pos_.x < before_mouse_pos_.x)
		{
			float constant = before_mouse_pos_.x - now_mouse_pos_.x;

			if (constant > kMaxMouseDiff)
			{
				constant = kMaxMouseDiff;
			}

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


	velocity_.x = direction_.x * side_distance_;
	velocity_.z = direction_.z * side_distance_;

}


void Brain::ChangeCamera()
{
	switch (change_type_)
	{

	case ChangeType::Straight:

		

		break;

	case ChangeType::Turn:



		break;


	}
}


void Brain::SetPos(const VECTOR& pos, const VECTOR& next_pos,const ChangeType& change_type)
{
	pos_ = pos;
	next_pos_ = next_pos;
	is_change_ = TRUE;
	change_type_ = change_type;
}