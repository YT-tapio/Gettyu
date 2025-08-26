#include<iostream>
#include"DxLib.h"
#include"player.h"
#include"camera.h"
#include"screen.h"
#include"brain.h"

#include"input.h"

#define _USE_MATH_DEFINES
#include <math.h>

Brain::Brain(const VECTOR& next_target_pos)
	:pos_(VGet(0,0,0))
	,next_pos_(next_target_pos)
	,next_target_pos_(VGet(0,0,0))
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
}


VECTOR Brain::OffsetPassingVel(const VECTOR& now_pos, const VECTOR& target_pos, const float& speed)
{
	VECTOR vel = VGet(0,0,0);

	if (now_pos.x == target_pos.x && now_pos.y == target_pos.y && now_pos.z == target_pos.z)
	{
		return vel;
	}

	//移動量を決める
	vel = VScale(VNorm(GetFutureToNowPositionVelocity(target_pos, now_pos)), speed);

	VECTOR future_pos = VAdd(now_pos, vel);
	VECTOR target_to_now;
	bool is_offset = FALSE;

	if(TRUE)
	{
		//現在のposとターゲットの関係を調べる
	/*---x---*/

		//今の座標がターゲットより小さいかつ未来の座標がターゲットより大きい
		if (now_pos.x < target_pos.x && target_pos.x < future_pos.x)
		{
			vel.x = target_pos.x - now_pos.x;
			is_offset = TRUE;
		}

		//今の座標がターゲットよりもとき大きいかつ未来の座標がターゲットよりも小さいとき
		if (future_pos.x < target_pos.x && target_pos.x < now_pos.x)
		{
			vel.x = target_pos.x - now_pos.x;
			is_offset = TRUE;
		}


		/*--y--*/

		//今の座標がターゲットより小さいかつ未来の座標がターゲットより大きい
		if (now_pos.y < target_pos.y && target_pos.y < future_pos.y)
		{
			vel.y = target_pos.y - now_pos.y;
			is_offset = TRUE;
		}

		//今の座標がターゲットよりもき大きいかつ未来の座標がターゲットよりも小さいとき
		if (future_pos.y < target_pos.y && target_pos.y < now_pos.y)
		{
			vel.y = target_pos.y- now_pos.y;
			is_offset = TRUE;
		}


		/*--z--*/

		//今の座標がターゲットより小さいかつ未来の座標がターゲットより大きい
		if (now_pos.z < target_pos.z && target_pos.z < future_pos.z)
		{

			vel.z = target_pos.z - now_pos.z;
			is_offset = TRUE;
		}

		//今の座標がターゲットよりもき大きいかつ未来の座標がターゲットよりも小さいとき
		if (future_pos.z < target_pos.z && target_pos.z < now_pos.z)
		{
			vel.z = target_pos.z - now_pos.z;
			is_offset = TRUE;
		}
	}
	else
	{
		//外積の値が0なら衝突(|V1*v|)

		target_to_now = GetFutureToNowPositionVelocity(target_pos, now_pos);

		VECTOR product;

		product.x = ((vel.y * target_to_now.z) - (vel.z * target_to_now.y));
		product.y = ((vel.z * target_to_now.x) - (vel.x * target_to_now.z));
		product.z = ((vel.x * target_to_now.y) - (vel.y * target_to_now.x));

		if (sqrt((product.x * product.x) + (product.y * product.y) + (product.z * product.z)) == 0)
		{
			vel = GetFutureToNowPositionVelocity(target_pos, now_pos);
		}
	}
	
	


	

	return vel;
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

void Brain::Update(const VECTOR& now_target_pos,const VECTOR& camera_pos, std::shared_ptr<Player> player)
{
	float speed = 1.0f;
	//球体上に回る処理のターゲット
	VECTOR sphere_target_pos = player->GetCenterPos();
	no_update_ = TRUE;



	if (player->GetIsSuperAttack())
	{
		SuperAttackUpdate(camera_pos, now_target_pos,player);
		speed = kSuperAttackCameraMoveSpeed;
		target_velocity_ = OffsetPassingVel(now_target_pos, next_target_pos_, 0.5f);
	}
	else
	{
		SphereUpdate(sphere_target_pos, camera_pos, player->GetInput());
		speed = 1.0f;
		target_velocity_ = VSub(sphere_target_pos, now_target_pos);
	}

	if (no_update_)
	{
		//正面を決めれたので、今のposから次のposまでのオフセットをする
		velocity_ = OffsetPassingVel(camera_pos, next_pos_, speed);
	}
	//printfDx("x:%f,y:%f,z:%f\n",now_target_pos.x,now_target_pos.y, now_target_pos.z);
	
}



void Brain::SphereUpdate(const VECTOR& target_pos,const VECTOR& camera_pos,const Input* input)
{

	Input* inp = new Input(input->GetPadNom());
	inp->SetTypeState(input->GetNowTypeState(), input->GetBeforeTypeState());

	float pad_side_rad_value = 0.0f;
	float pad_vertical_rad_value = 0.0f;

	float mouse_side_rad_value = 0.0f;
	float mouse_vertical_rad_value = 0.0f;

	float decide_side_rad_value = 0.0f;
	float decide_vertical_rad_value = 0.0f;

	pad_side_rad_value = static_cast<float>((M_PI / 180) * (inp->GetPadStickPercent(StickType::kRight, Control::kX) * kCameraSpeed) * all_sensitivity_) * side_sensitivity_;;
	pad_vertical_rad_value = -(static_cast<float>((M_PI / 180) * (inp->GetPadStickPercent(StickType::kRight, Control::kY) * kCameraSpeed) * all_sensitivity_) * vertical_sensitivity_);

	//if()
	mouse_side_rad_value = static_cast<float>((M_PI / 180) * (inp->GetMousePercent(Control::kX) * kCameraSpeed) * all_sensitivity_) * side_sensitivity_;
	mouse_vertical_rad_value = static_cast<float>((M_PI / 180) * (inp->GetMousePercent(Control::kY) * kCameraSpeed) * all_sensitivity_) * vertical_sensitivity_;

	inp->ResetMousePoint();


	if (pad_side_rad_value == 0.0f && pad_vertical_rad_value == 0.0f)
	{
		decide_side_rad_value = mouse_side_rad_value;
		decide_vertical_rad_value = mouse_vertical_rad_value;
	}

	if (mouse_side_rad_value == 0.0f && mouse_vertical_rad_value == 0.0f)
	{
		decide_side_rad_value = pad_side_rad_value;
		decide_vertical_rad_value = pad_vertical_rad_value;
		
	}

	if (mouse_side_rad_value == 0.0f && mouse_vertical_rad_value == 0.0f &&
		pad_side_rad_value == 0.0f && pad_vertical_rad_value == 0.0f)
	{
		no_update_ = TRUE;
	}
	else
	{
		no_update_ = FALSE;
	}

	//pad対応
	side_rad_ += decide_side_rad_value;
	vertical_rad_ += decide_vertical_rad_value;	//pad操作の時、カメラを動かすときは上下が反転する
	
	//side_radの調整
	if (side_rad_ > static_cast<float>((M_PI / 180) * 180))
	{
		side_rad_ -= static_cast<float>((M_PI / 180) * 360);
	}

	if (side_rad_ < -static_cast<float>((M_PI / 180) * 180))
	{
		side_rad_ += static_cast<float>((M_PI / 180) * 360);
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

	direction_.x = sinf(side_rad_);
	direction_.z = cosf(side_rad_);


	/*----横の長さをだす(cos)---*/

	velocity_.y = (sinf(vertical_rad_)) * distance_;
	side_distance_ = (cosf(vertical_rad_)) * distance_;

	velocity_.x = direction_.x * side_distance_;
	velocity_.z = direction_.z * side_distance_;

	next_pos_ = VAdd(target_pos, velocity_);
	
	velocity_ = GetFutureToNowPositionVelocity(next_pos_, camera_pos);

}


void Brain::SuperAttackUpdate(const VECTOR& camera_pos, const VECTOR& now_target_pos, std::shared_ptr<Player> player)
{
	//条件分岐()
	switch (player->GetNowCameraSituationNum())
	{
	case 0:

		//とりあえずプレイヤーの正面に行く処理
		VECTOR front_pos;

		//プレイヤーのpos,rotationを受け取る
		VECTOR pos = VScale(VAdd(player->GetCapsuleData().start_pos, player->GetCapsuleData().end_pos), 0.5f);			//基準のポジション
		VECTOR rot = player->GetRotation();		//プレイヤーの回転量

		//今角度がわかっている状態、どんだけ離れているのかも位知っている(極座標がわかっている)
		front_pos = pos;

		//正面にカメラを持ってきたいので180度プラスする
		rot.y += static_cast<float>((M_PI / 180) * 180);

		if (rot.y > static_cast<float>((M_PI / 180) * 180))
		{
			rot.y = rot.y - (static_cast<float>((M_PI / 180) * 360));
		}

		front_pos.x += (sinf(rot.y) * kSuperAttackZeroDist);
		front_pos.z += (cosf(rot.y) * kSuperAttackZeroDist);

		next_pos_ = front_pos;

		//ポジションが一致したとき、次のカメラに切り替える
		if (CheckSamePos(camera_pos, next_pos_))
		{
			next_target_pos_ = player->GetWeaponPos();
			//一緒にはならない、許容の範囲を作る
			if (VSize(GetFutureToNowPositionVelocity(next_target_pos_,now_target_pos)) < 0.25)
			{
				player->SetNowCameraSituation(1);
			}

		}
		else
		{
			next_target_pos_ = player->GetCenterPos();
		}



		break;

	case 1:

		//二回目はキャラクターの上を見る
		next_target_pos_ = player->GetSuperAttackEffectPosition();
		//見たら座標を移動
		if (VSize(GetFutureToNowPositionVelocity(next_target_pos_, now_target_pos)) < 0.25)
		{
			//エフェクトの少し上へ移動
			//真上に行くと描画ができなくなるので少しずらす

			VECTOR offset_vel = VGet(0, 0, 0);

			offset_vel.x = (sinf(static_cast<float>((M_PI / 180) * kSuperAttackFirstSideRad)) * kSuperAttackFirstDist);
			offset_vel.z = (cosf(static_cast<float>((M_PI / 180) * kSuperAttackFirstSideRad)) * kSuperAttackFirstDist);

			offset_vel = VAdd(offset_vel, VGet(0, 20, 0));

			next_pos_ = VAdd(player->GetSuperAttackEffectPosition(), offset_vel);
		}

		break;

	case 2:

		break;

	case 3:

		break;

	}
	

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