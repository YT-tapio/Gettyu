#include<iostream>
#include"DxLib.h"
#include"camera.h"
#include"screen.h"
#include"Calculation.h"
#include"brain.h"
#include"input.h"

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

void Brain::MakeVertical()
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

		vertical_rad_ += static_cast<float>((M_PI / 180) * (constant / 10) * all_sensitivity_) * vertical_sensitivity_;
	}

	//下に行くとき
	if (now_mouse_pos_.y < before_mouse_pos_.y)
	{
		float constant = before_mouse_pos_.y - now_mouse_pos_.y;

		if (constant > kMaxMouseDiff)
		{
			constant = kMaxMouseDiff;
		}

		vertical_rad_ -= static_cast<float>((M_PI / 180) * (constant / 10) * all_sensitivity_) * vertical_sensitivity_;
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


VECTOR Brain::GetVelocityDecidedRad()
{
	VECTOR vel;

	vel.y = (sinf(vertical_rad_)) * distance_;
	side_distance_ = (cosf(vertical_rad_)) * distance_;
	
	vel.x = (sinf(side_rad_)) * side_distance_;
	vel.z = (cosf(side_rad_)) * side_distance_;

	return vel;

}


/*---------------public---------------*/

void Brain::Update(const VECTOR& target_pos,const VECTOR& camera_pos,const Input* input)
{
	SphereUpdate(target_pos, camera_pos, input);
}




void Brain::SphereUpdate(const VECTOR& target_pos,const VECTOR& camera_pos,const Input* input)
{

	Input* inp = new Input(input->GetPadNom());
	inp->SetTypeState(input->GetNowTypeState(), input->GetBeforeTypeState());

	//マウスポインターの取得
	GetMousePoint(&now_mouse_pos_.x, &now_mouse_pos_.y);


	//前回と現在のポインターの位置が違うとき
	if (CheckMousePoint(now_mouse_pos_, before_mouse_pos_))
	{
		//横の回転が360を超えないように
		if (side_rad_ > (M_PI * 2)) { side_rad_ = 0; }

		//縦の回転を作る
		MakeVertical();

		//
		if (now_mouse_pos_.x > before_mouse_pos_.x)
		{
			float constant = now_mouse_pos_.x - before_mouse_pos_.x;

			if (constant > kMaxMouseDiff)
			{
				constant = kMaxMouseDiff;
			}

			side_rad_ += static_cast<float>((M_PI / 180) * (constant / 10) * all_sensitivity_) * side_sensitivity_;
		}

		if (now_mouse_pos_.x < before_mouse_pos_.x)
		{
			float constant = before_mouse_pos_.x - now_mouse_pos_.x;

			if (constant > kMaxMouseDiff)
			{
				constant = kMaxMouseDiff;
			}

			side_rad_ -= static_cast<float>((M_PI / 180) * (constant / 10) * all_sensitivity_) * side_sensitivity_;
		}


		//pos_.y = 40;

		int MouseX = (float(kGameWidth) * 0.5f), MouseY = (float(kGameHeight) * 0.5f);
		SetMousePoint(MouseX, MouseY);
		GetMousePoint(&now_mouse_pos_.x, &now_mouse_pos_.y);
		before_mouse_pos_ = now_mouse_pos_;
	}

	float pad_vertical_num = inp->GetPadStickVertical(StickType::kRight);

	if (pad_vertical_num > 50.0f)
	{


	}
	

	direction_.x = sinf(side_rad_);
	direction_.z = cosf(side_rad_);

	/*----横の長さをだす(cos)---*/

	velocity_.y = (sinf(vertical_rad_)) * distance_;
	side_distance_ = (cosf(vertical_rad_)) * distance_;

	velocity_.x = direction_.x * side_distance_;
	velocity_.z = direction_.z * side_distance_;

	VECTOR future_pos = VAdd(target_pos, velocity_);
	//velocityの調整を行う(未来の座標と今の座標の距離を測る)
	velocity_ = GetFutureToNowPositionVelocity(future_pos, camera_pos);

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


void Brain::SetRad(const VECTOR& target_pos, const VECTOR& player_pos)
{
	//新しいVECTORを作る(rotation)
	VECTOR rot_vec = VGet(target_pos.x - player_pos.x, 0.0f, target_pos.z - player_pos.z);
	
	//タンジェントの解を求める
	float tan_num = 0;

	if (rot_vec.x == 0.0f)
	{
		if (rot_vec.z > 0.0f)
		{
			side_rad_ = static_cast<float>((M_PI / 180) * 90);
		}
		else
		{
			side_rad_ = -1 * (static_cast<float>((M_PI / 180) * 90));
		}

	}
	else
	{
		side_rad_ = static_cast<float>((M_PI / 180) * 180) +atan2f(rot_vec.x, rot_vec.z);
	}

	vertical_rad_ = static_cast<float>((M_PI / 180) * 30);
	
	int MouseX = (float(kGameWidth) * 0.5f), MouseY = (float(kGameHeight) * 0.5f);
	SetMousePoint(MouseX, MouseY);

}

void Brain::SetVelocity(const VECTOR& target_pos,const VECTOR& camera_pos)
{
	
	velocity_ = GetVelocityDecidedRad();

	VECTOR future_pos = VAdd(target_pos, velocity_);

	velocity_ = GetFutureToNowPositionVelocity(future_pos, camera_pos);
	
	velocity_ = OffsetVelocity(velocity_, 3.0f);
}


VECTOR Brain::GetFutureToNowPositionVelocity(const VECTOR& future_pos, const VECTOR& now_pos)
{
	return VGet(future_pos.x - now_pos.x, future_pos.y - now_pos.y, future_pos.z - now_pos.z);
}


VECTOR Brain::OffsetVelocity(const VECTOR& velocity,float offset_num)
{
	VECTOR vel = velocity;

	//既定の移動量を超えるなら
	if (VSize(velocity) > offset_num)
	{
		VECTOR norm_vel = VNorm(velocity);
		vel = VScale(norm_vel, offset_num);
	}


	return vel;


}


VECTOR Brain::GetPositionFromTarget(const VECTOR& target_pos)
{
	VECTOR pos;				//求めたい位置
	VECTOR velocity;		//ターゲットからの距離
	float side_dis;			//地面との平衡の距離
	
	//高さを出す
	velocity.y = (distance_ * sinf(vertical_rad_));

	//地面の距離を出す
	side_distance_ = (distance_ * cosf(vertical_rad_));

	//地面のポジションを出す
	velocity.x = (side_distance_ * cosf(side_rad_));		//xを求めるにはcos
	velocity.z = (side_distance_ * sinf(side_rad_));		//zを求めるにはsin

	//ターゲットのポジションにたす
	pos = VAdd(target_pos, velocity);

	return pos;
}


void Brain::Draw()
{
	DrawFormatString(200, 200, GetColor(255, 0, 0), "%f", side_rad_);
	DrawFormatString(200, 220, GetColor(255, 0, 0), "%f", vertical_rad_);

	float side_rad_not_pi = side_rad_ / (M_PI / 180);
	float vertical_rad_not_pi = vertical_rad_ / (M_PI / 180);

	DrawFormatString(200, 240, GetColor(255, 0, 0), "%f", side_rad_not_pi);
	DrawFormatString(200, 260, GetColor(255, 0, 0), "%f", vertical_rad_not_pi);
}