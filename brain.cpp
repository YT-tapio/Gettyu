#include<iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include"DxLib.h"
#include"player.h"
#include"weapon_base.h"
#include"camera.h"
#include"screen.h"
#include"brain.h"
#include"input.h"
#include"situation.h"
#include"tracking.h"

Brain::Brain(const VECTOR& next_target_pos)
	:pos_(VGet(0,0,0))
	,start_pos_(VGet(0,0,0))
	,next_pos_(next_target_pos)
	,next_target_pos_(VGet(0,0,0))
	,get_camera_center_pos_(VGet(0,0,0))
	,is_change_(FALSE)
	,is_blend_(FALSE)
	,is_target_blend_(FALSE)
	,change_type_(ChangeType::Straight)
	,camera_name_(VirtualCameraName::kNothing)
	,vibration_count_(0)
	,super_attack_vibration_power_(500)
	,side_rad_(static_cast<float>(M_PI / 180) * 0.f)
	,vertical_rad_(static_cast<float>(M_PI / 180) * 10.f)
{
	sphere_camera_ = new SphereCamera(VirtualCameraName::kSphere);
	get_camera_ = new GetCamera(VirtualCameraName::kGet);
	super_attack_camera_[0] = new SuperAttackCamera(VirtualCameraName::kSuperAttackFirst);
	super_attack_camera_[1] = new SuperAttackCamera(VirtualCameraName::kSuperAttackSecond);
	super_attack_camera_[2] = new SuperAttackCamera(VirtualCameraName::kSuperAttackThird);
	tracking_camera_ = new Tracking(VirtualCameraName::kTracking);
}


Brain::~Brain()
{
	delete sphere_camera_;
	delete get_camera_;
	delete super_attack_camera_[0];
	delete super_attack_camera_[1];
	delete super_attack_camera_[2];
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


VECTOR Brain::OffsetPassingVel(const VECTOR& now_pos, const VECTOR& target_pos,const VECTOR& velocity, bool& flag)
{
	VECTOR vel = velocity;
	
	//未来の位置
	VECTOR future_pos = VAdd(now_pos, vel);
	
	bool offseted = FALSE;

	//今の座標と未来の座標をtarget_posを基軸に比べる

	//今はtargetより小さく未来がtagetよりも大きいとき
	if (now_pos.x < target_pos.x && target_pos.x < future_pos.x && !(offseted))
	{
		vel = VGet(0, 0, 0);
		vel = VSub(target_pos, now_pos);
		flag = FALSE;
		offseted = TRUE;
	}
	
	if (now_pos.y < target_pos.y && target_pos.y < future_pos.y && !(offseted))
	{
		vel = VSub(target_pos, now_pos);
		flag = FALSE;
		offseted = TRUE;
	}

	if (now_pos.z < target_pos.z && target_pos.z < future_pos.z && !(offseted))
	{
		vel = VSub(target_pos, now_pos);
		flag = FALSE;
		offseted = TRUE;
	}

	//今はtargetより大きく未来がtagetよりも小さいとき
	if (now_pos.x > target_pos.x && target_pos.x > future_pos.x && !(offseted))
	{
		vel = VSub(target_pos, now_pos);
		flag = FALSE;
		offseted = TRUE;

	}

	if (now_pos.y > target_pos.y && target_pos.y > future_pos.y && !(offseted))
	{
		vel = VSub(target_pos, now_pos);
		flag = FALSE;
		offseted = TRUE;
	}

	if (now_pos.z > target_pos.z && target_pos.z > future_pos.z && !(offseted))
	{
		vel = VSub(target_pos, now_pos);
		flag = FALSE;
		offseted = TRUE;
	}


	return vel;
}


VECTOR Brain::SetSuperAttackFrontPos(std::shared_ptr<Player> player)
{
	VECTOR vel = VGet(0, 0, 0);
	VECTOR front_pos = VGet(0,0,0);

	float rot = (player->GetRotation().y + static_cast<float>((M_PI / 180) * 180));

	if (rot > static_cast<float>((M_PI / 180) * 180))
	{
		rot = rot - (static_cast<float>((M_PI / 180) * 360));
	}

	vel.x = (sinf(rot) * kSuperAttackZeroDist);
	vel.z = (cosf(rot) * kSuperAttackZeroDist);


	//playerの今の位置からvelをたす
	front_pos = VAdd(player->GetPos(), vel);



	return front_pos;
}


VECTOR Brain::GetThisDistanceOfffsetPos(const VECTOR& pos, const float& distance, const float& ver_rad, const float& side_rad,std::shared_ptr<Player> player)
{
	VECTOR offset_vel = VGet(0, 0, 0);

	float side_distance = 0.0f;

	/*----横の長さをだす(cos)---*/
	//高さがどれくらいか
	//地上の長さがどれくらいか
	offset_vel.y = sinf(static_cast<float>((M_PI / 180 ) * ver_rad)) * distance;
	side_distance = cosf(static_cast<float>((M_PI / 180) * ver_rad)) * distance;

	offset_vel.x = sinf(static_cast<float>((M_PI / 180) * side_rad) 
		+ player->GetRotation().y) * side_distance;
	offset_vel.z = cosf(static_cast<float>((M_PI / 180) * side_rad)
		+ player->GetRotation().y) * side_distance;

	
	return VAdd(pos, offset_vel);
}


VECTOR Brain::GetSuperAttackEffectBehindPos(std::shared_ptr<Player> player)
{
	//エフェクトの後ろのポジションを指定する
	//エフェクトの少し上へ移動
	//真上に行くと描画ができなくなるので少しずらす

	VECTOR offset_vel = VGet(0, 0, 0);

	offset_vel.x = (sinf(static_cast<float>((M_PI / 180) * 
		kSuperAttackFirstSideRad) + player->GetRotation().y) * kSuperAttackFirstDist);
	offset_vel.z = (cosf(static_cast<float>((M_PI / 180) * 
		kSuperAttackFirstSideRad) + player->GetRotation().y) * kSuperAttackFirstDist);



	offset_vel = VAdd(offset_vel, VGet(0, 10, 0));

	//エフェクトの後ろに移動
	return (VAdd(player->GetSuperAttackEffectPosition(), offset_vel));
}


VECTOR Brain::GetSuperAttackWeaponCameraPos(const VECTOR& pos)
{
	//weaponの位置
	VECTOR weapon_pos = pos;
	

	//wraponの位置からの距離を出す
	VECTOR offset_vel;



	return VGet(0,0,0);
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


void Brain::Init(const VECTOR& camera_pos,const VECTOR& player_pos)
{

	//ここで最初のカメラのポジションの指定をする
	

	//sphereのようなことをします

	//side_radとvertical_radをきめて

	//だんだんとdistを小さくしていくそしてゲットになる

	VECTOR next_pos = VGet(0.f, 0.f, 0.f);
	float side_dist = 0.f;				//地面のdist


	//directionを決めてからにしましょう

	direction_.x = sinf(side_rad_);
	direction_.z = cosf(side_rad_);

	velocity_.y = distance_ * sinf(vertical_rad_);
	side_dist= distance_ * cosf(vertical_rad_);

	velocity_.x = direction_.x * side_distance_;
	velocity_.z = direction_.z * side_distance_;

	next_pos = VAdd(player_pos, velocity_);

	velocity_ = VSub(next_pos, camera_pos);
}


void Brain::GetInit(const VECTOR& camera_pos, const VECTOR& target_dir)
{
	//ここでゲットした時のinitを行う
	//enemyの正面に行きたい


	//うけとったdirのdist分をtarget_posにします

	float enemy_dist = 10.0f;


	//2つのポジションを地面に添わせる
	VECTOR front_pos = VScale(target_dir, enemy_dist);

	VECTOR cam_on_the_line_pos = VGet(camera_pos.x, 0.f, camera_pos.z);
	VECTOR front_on_the_line_pos = VGet(front_pos.x, 0.f, front_pos.z);

	

	VECTOR camera_to_enemy_vel = VSub(cam_on_the_line_pos, front_on_the_line_pos);

	//camera_posからfront_posを引きどんくらいはなれているかをみてsizeを取得する
	camera_to_enemy_dist_ = VSize(camera_to_enemy_vel);

	//どのくらいの距離(高さ)も取得
	camera_to_enemy_height_ = camera_pos.y - front_pos.y;

	//センターのポジションを決めなきゃ
	//distの半分

	//cameraからenemyのvelの半分をcamera_posに足せばok
	get_camera_center_pos_ = VAdd(camera_pos, VScale(camera_to_enemy_vel, 0.5f));

	//アークタンジェントによって求める
	get_camera_init_rad_ = atan2f(front_pos.z, front_pos.x);



	printfDx("%.2f\n", get_camera_init_rad_);

	//求められたやつを180どぶん足してあげる

	

}


void Brain::Update(const VECTOR& now_target_pos,const VECTOR& camera_pos, std::shared_ptr<Player> player)
{
	float speed = 1.0f;
	//球体上に回る処理のターゲット
	VECTOR sphere_target_pos = player->GetCenterPos();
	no_update_ = TRUE;

	bool next_is_blend = FALSE;

	static int before_camera_name = VirtualCameraName::kNothing;

	//必殺技のカメラを識別
	static int super_attack_situation_num = 0;

	//移動量のリセット
	velocity_ = VGet(0.f, 0.f, 0.f);
	target_velocity_ = VGet(0.f, 0.f, 0.f);

	// バーチャルカメラのUpdate
	// next_posにsphereの結果やsuperattackの座標を入れる
	// カメラを切り替えたという情報が欲しい
	// 切り替えがわかるとblendを行う
	// バーチャルカメラにアップデート持たせてもいいんじゃない(無しになりました)
	// そいつがカメラとして設定されているときはそいつのUpdateを回してvelocityを受け取る



	//switchで管理しておく
	//各virtual_cameraにname_があるので、それを受け取る

	//何もないとき(kNothing)は、Trackingに切り替える
	if (camera_name_ == VirtualCameraName::kNothing) 
	{ 
		camera_name_ = sphere_camera_->GetCameraName();
		if (before_camera_name == VirtualCameraName::kNothing)
		{
			before_camera_name = camera_name_;
		}
	}

	//各カメラに名前を持たせる
	//もし、kSuperAttack以上なら

	
	
	//必殺技じゃないときに
	if (camera_name_ < VirtualCameraName::kSuperAttack)
	{
		//必殺中だと
		if (player->GetIsSuperAttack())
		{
			camera_name_ = super_attack_camera_[0]->GetCameraName();
		}
		else  //必殺ではないとき
		{
			//camera_name_ = sphere_camera_->GetCameraName();
		}
	}

	
	
	//カメラの処理を変える
	//上に行かないで注視点だけを変えたい


	//situationがゲットの時にvirtualcameraを切り替える

	if (Situation::GetInstance().GetSituationName() == SituationName::kGet)
	{
		//getカメラに切り替える
		camera_name_ = VirtualCameraName::kGet;
	}


	//前回と結果が違う(カメラが切り替わる)ときblendさせる
	if (camera_name_ != before_camera_name)
	{
		is_blend_ = TRUE;
		
		//今の座標と次のvirtualcameraの座標をとる
		start_pos_ = camera_pos;

		if (camera_name_ == VirtualCameraName::kGet)
		{
			is_blend_ = FALSE;
		}

		//Initする
		//ここでblendなどの調整する
		//位置補正の調整など
		switch (camera_name_)
		{
		case VirtualCameraName::kSphere:

			next_pos_ = sphere_camera_->GetPos();
			is_target_blend_ = TRUE;
			sphere_camera_->SetTargetPos(player->GetCenterPos());
			start_target_pos_ = now_target_pos;
			next_target_pos_ = player->GetCenterPos();


			//getからsphereに代わるときは違う処理にする
			
			if (before_camera_name == VirtualCameraName::kGet)
			{
				is_blend_ = FALSE;
				is_target_blend_ = FALSE;
			}


			break;


		case VirtualCameraName::kGet:

			player->Vibration(500, 1000);


			//ここでdistを決めたりする
			GetInit(camera_pos,VGet(-30.f,0.f,-10.f));


			break;



		case VirtualCameraName::kTracking:
			
			// ついてくるカメラですこれは
			// プレイヤーの正面には


			break;

			//プレイヤーの正面
		case VirtualCameraName::kSuperAttackFirst:

			//プレイヤーの正面の座標を受け取る
			super_attack_camera_[0]->SetPos(SetSuperAttackFrontPos(player));
			super_attack_camera_[0]->SetTargetPos(player->GetSuperAttackEffectPosition());
			next_pos_ = super_attack_camera_[0]->GetPos();
			start_target_pos_ = now_target_pos;
			next_target_pos_ = super_attack_camera_[0]->GetTargetPos();
			is_target_blend_ = TRUE;

			//blend_speedを入れる

			blend_speed_ = 10.0f;

			//printfDx("\nx:%.2f,y:%.2f,z:%.2f\n", next_pos_.x, next_pos_.y, next_pos_.z);
			//printfDx("x:%.2f,y:%.2f,z:%.2f\n", player->GetPos().x, player->GetPos().y, player->GetPos().z);
			break;


		case VirtualCameraName::kSuperAttackSecond:

			// カメラの位置は正面のままでok
			// ターゲットの位置だけ変える
			// ターゲットの位置はプレイヤーの位置を見る

			super_attack_camera_[1]->SetPos(SetSuperAttackFrontPos(player));
			super_attack_camera_[1]->SetTargetPos(player->GetCenterPos());
			next_pos_ = super_attack_camera_[1]->GetPos();
			start_target_pos_ = now_target_pos;
			next_target_pos_ = super_attack_camera_[1]->GetTargetPos();
			
			//必殺中はblendが切り替わった瞬間にtagをかえているけど、例外としてここでtagを変えておく
			is_blend_ = FALSE;
			is_target_blend_ = TRUE;

			

			player->SetNowCameraSituation(camera_name_ - VirtualCameraName::kSuperAttackFirst);


			break;


			

		}

		before_camera_name = camera_name_;

	}



	// blend中じゃないときはswitchで管理
	if (!is_blend_)
	{
		switch (camera_name_)
		{
		case VirtualCameraName::kSphere:

			SphereUpdate(sphere_target_pos, camera_pos, player->GetInput());
			speed = 1.0f;

			if (is_target_blend_)
			{
				target_velocity_ = GetStartToNextVelocity(start_target_pos_, now_target_pos, next_target_pos_, target_blend_speed_, is_target_blend_);
			}
			else
			{
				target_velocity_ = VSub(sphere_target_pos, now_target_pos);
			}
			
			

			break;

		case VirtualCameraName::kGet:

			GetCameraUpdate(Situation::GetInstance().GetSituationPos(), camera_pos, now_target_pos);

			break;


		case VirtualCameraName::kTracking:

			TrackingUpdate(camera_pos, player);

			break;


		case VirtualCameraName::kSuperAttackFirst:

			if (is_target_blend_)
			{
				target_velocity_ = GetStartToNextVelocity(start_target_pos_, now_target_pos,
					next_target_pos_, target_blend_speed_, is_target_blend_);
			}
			//どちらのブレンドも終わったら
			if (!is_blend_ && !is_target_blend_)
			{
				camera_name_++;
			}
			//見る位置のoffsetを開始する

			break;

		case VirtualCameraName::kSuperAttackSecond:

			if (is_target_blend_)
			{
				target_velocity_ = GetStartToNextVelocity(start_target_pos_, now_target_pos,
					next_target_pos_, target_blend_speed_, is_target_blend_);
			}
			else
			{

				//printfDx("%.2f\n", player->GetSuperAttackEffectPlayCount());
				player->AttachWeapon(WeaponName::kWizardStaff);
				//player->Vibration(super_attack_vibration_power_,10);
				Vibration();
				if (CheckHitKey(KEY_INPUT_Y) || player->GetSuperAttackEffectPlayCount() > 130.f)
				{
					//たーげっとのブレンドも終わってえふぇくとも終わると切り替える
					camera_name_ = VirtualCameraName::kNothing;
					player->SetIsSuperAttack(FALSE);

					player->Vibration(1000, 100);
				}
				else
				{
					player->Vibration(500, 300);
				}	
			}

			break;
		
		}
	}
	else
	{
		//位置をブレンド(滑らかにするために)
		//関数を呼び出して、velocityに直で入れる

		//関数は引数でブレンドを開始した位置と行きたい位置とどんくらい(speed)で行くかを受け取り、velocityを調整する
		velocity_ = GetStartToNextVelocity(start_pos_, camera_pos, next_pos_, blend_speed_,is_blend_);

		if (!is_blend_)
		{
			if (camera_name_ > VirtualCameraName::kSuperAttack)
			{
				player->SetNowCameraSituation(camera_name_ - VirtualCameraName::kSuperAttackFirst);
			}
		}
	}


	


	player->SetIsBlend(is_blend_);
	player->SetIsTargetBlend(is_target_blend_);
	
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

	pad_side_rad_value = static_cast<float>(((M_PI / 180) * (inp->GetPadStickPercent(StickType::kRight, Control::kX) * kCameraSpeed) * all_sensitivity_) * side_sensitivity_) * 0.5f;
	pad_vertical_rad_value = -(static_cast<float>((M_PI / 180) * ((inp->GetPadStickPercent(StickType::kRight, Control::kY) * kCameraSpeed) * all_sensitivity_) * vertical_sensitivity_)) * 0.5f;

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
		//decide_side_rad_value = pad_side_rad_value;
		//decide_vertical_rad_value = pad_vertical_rad_value;
		
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
	side_rad_ += decide_side_rad_value * (delta_time_ * 20);
	vertical_rad_ += decide_vertical_rad_value * (delta_time_ * 20);	//pad操作の時、カメラを動かすときは上下が反転する
	
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

	//カメラの位置を記憶
	sphere_camera_->SetPos(VAdd(camera_pos, velocity_));


	//カメラの位置を記憶
	pos_ = VAdd(camera_pos, velocity_);

}


void Brain::TrackingUpdate(const VECTOR& now_camera_pos,std::shared_ptr<Player> player)
{
	
	//引数にカメラの現在のポジションとplayerをそのまま持ってくる


	//ここで追尾の更新をする

	//サルゲッチュの追尾のカメラは
	//カメラの正面に移動するならそのままついてくる
	//横移動の時はついてこなくなる
	//斜めの時はdir分はついてくる
	//playerとcameraが一定距離離れてしまうのならそのままのvelocity分追尾する
	//

	//maxのdistを決めておく
	const float kMaxDist = 50.f;

	//cameraとplayerの距離を見る
	
	VECTOR dist_vec = VGet(0.f, 0.f, 0.f);


	dist_vec = VSub(now_camera_pos, player->GetPos());

	//とりあえずそのままついてくるようにする,target_velocityも
	velocity_ = player->GetVelocity();
	target_velocity_ = player->GetVelocity();
	return;

	//マックスの距離離れるならそのままplayerのvelocityを渡してあげる
	if (VSize(dist_vec) >= kMaxDist)
	{

		//playerのvelocityを受け取る
		velocity_ = player->GetVelocity();
		return;
	}



	//playerのvelocityをもらう


}



void Brain::SuperAttackUpdate(const VECTOR& camera_pos, const VECTOR& now_target_pos, std::shared_ptr<Player> player)
{
	//条件分岐()
	switch (player->GetNowCameraSituationNum())
	{
	case 0:

		//移動量を受け取る
		VECTOR vel = VGet(0, 0, 0);

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

		vel.x = (sinf(rot.y) * kSuperAttackZeroDist);
		vel.z = (cosf(rot.y) * kSuperAttackZeroDist);

		front_pos.x += vel.x;
		front_pos.z += vel.z;

		super_attack_camera_[0]->SetPos(front_pos);

		

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


		if (is_blend_)
		{
			//velocity_ = GetStartToNextVelocity(pos_, camera_pos, super_attack_camera_num_first_->GetPos(), 3.f);
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
			
			//座標が一緒になると次へ
			if (CheckSamePos(camera_pos, next_pos_))
			{
				player->SetNowCameraSituation(2);
			}


		}

		

		break;

	case 2:

		

		break;

	case 3:

		break;

	}
	

}


void Brain::GetCameraUpdate(const VECTOR& pos, const VECTOR& camera_pos,const VECTOR& target_pos)
{
	if (FALSE)
	{

		const int kCountMax = 10;
		static int  now_count = 0;
		//かめらのさいしょのrad
		get_camera_init_rad_;
		camera_to_enemy_dist_;
		//カメラと中心のポジション
		get_camera_center_pos_;
		//高さ
		camera_to_enemy_height_;


		//条件満たしたらcount初期化



	}
	else
	{
		//とりあえず回る処理を作っていきたいです
	//ゲットじの処理を行います
	//球体上に回す
	//ゲットした対象を基軸に一定の距離分離す



	//とりあえず中心からの位置を出す
		static float rad = 30;
		const float kDist = 30.f;


		//回転量が定数以上行くときradも初期化する
		if (rad > 390.0f)
		{
			Situation::GetInstance().SetSituation(SituationName::kNothing);
			//カメラの切り替え
			camera_name_ = VirtualCameraName::kNothing;
			rad = 30;
		}


		//中心からの距離
		VECTOR dist_pos = VAdd(pos, VGet(cosf(static_cast<float>((M_PI / 180) * rad)) * kDist, 0.f,
			sinf(static_cast<float>((M_PI / 180) * rad)) * kDist));

		//距離を出す

		rad = rad + (40 * delta_time_);

		velocity_ = VSub(dist_pos, camera_pos);
		//注視点を変える
		target_velocity_ = VSub(pos, target_pos);


		//radが一定数に行くと切り替わる


		//カメラのターゲットをsituaionからターゲットを持ってくる

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


void Brain::Vibration()
{
	//左右を決めるやつ
	float side = 0.f;
	//とりあえず横にずらす
	vibration_count_++;

	//偶数
	if (vibration_count_ % 2 == 0)
	{
		vibration_count_ = 0;
		side = -0.1f;
	}
	else //奇数
	{
		side = 0.1f;
	}

	velocity_ = VAdd(velocity_, VGet(0.f, side, 0.f));
	target_velocity_ = VAdd(target_velocity_, VGet(0.f, side *10, 0.f));


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

	velocity_ = VScale(velocity_, delta_time_);

	VECTOR future_pos = VAdd(target_pos, velocity_);

	velocity_ = GetFutureToNowPositionVelocity(future_pos, camera_pos);
	
	velocity_ = OffsetVelocity(velocity_, 3.0f);
}


VECTOR Brain::GetStartToNextVelocity(const VECTOR& start_pos, const VECTOR& now_camera_pos,const VECTOR& next_pos, const float& time,bool& flag)
{
	VECTOR vel = VGet(0, 0, 0);

	if (now_camera_pos.x == next_pos.x &&
		now_camera_pos.y == next_pos.y &&
		now_camera_pos.z == next_pos.z)
	{
		flag = FALSE;
		return vel;
	}

	//あれでやってみようvel足す前と足した後でのやつを

	if (FALSE)
	{
		// 今の座標が一致しているとき
		if (CheckSamePos(now_camera_pos, next_pos))
		{
			is_blend_ = FALSE;
			//printfDx("とおだ");
			return vel;
		}

		// 各座標のdistance(VECTOR)の量を見る
		VECTOR this_to_next = GetFutureToNowPositionVelocity(next_pos, start_pos);

		float time_per = (delta_time_ / time);	//時間の比を見る

		vel = VScale(this_to_next, time_per);	//時間の比を全体の移動量にかける

		// 位置の先取りを行う
		if (CheckSamePos(VAdd(now_camera_pos, vel), next_pos))
		{
			printfDx("とおだ");
			//位置を少し調整
			is_blend_ = FALSE;
			return VSub(next_pos, VAdd(now_camera_pos, vel));
		}
	}
	else
	{
		// 各座標のdistance(VECTOR)の量を見る
		VECTOR this_to_next = GetFutureToNowPositionVelocity(next_pos, start_pos);

		float time_per = (delta_time_ / time);	//時間の比を見る

		vel = VScale(this_to_next, time_per);	//時間の比を全体の移動量にかける

		//

		vel = OffsetPassingVel(now_camera_pos, next_pos, vel, flag);
	}

	

	
	//時間の比を全体の距離にかける
	return vel;
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