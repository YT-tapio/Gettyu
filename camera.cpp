#include"camera.h"
#include"screen.h"
#include"EffekseerForDxLib.h"
#include"debug.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include"vector_assistant.h"

Camera::Camera()
{
	
}



void Camera::Init(const VECTOR& velocity)
{
	pos_ = VAdd(pos_, velocity);
}


void Camera::Awake(const VECTOR& pos, const VECTOR& target_pos, float fov)
{
	pos_					= VectorAssistant::GetZeroVec();
	target_pos_		= VectorAssistant::GetZeroVec();
	fov_					= fov;
	target_fov_		= 0.f;
	velocity_			= VectorAssistant::GetZeroVec();
	direction_			= VectorAssistant::GetZeroVec();

	before_near_				= kNear;
	before_far_				= kFar;
	before_fov_				= fov_;
	before_pos_				= pos_;
	before_target_pos_	= target_pos_;

	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);

	pos_ = pos;
	target_pos_ = target_pos;
	fov_ = fov;
	// 視野角設定
	SetupCamera_Perspective(fov_);
}


void Camera::Update(const VECTOR& velocity, const VECTOR& target_velocity)
{
	VECTOR vel = velocity;
	VECTOR target_vel = target_velocity;
	pos_ = VAdd(pos_, vel);
	target_pos_ = VAdd(target_pos_, target_vel);
	Effekseer_Sync3DSetting();
	
	SetLightPosition(pos_);
	//ターゲットの方向へのライトを出す
	VECTOR light_dir = VectorAssistant::GetDir(pos_, target_pos_);
	SetLightDirection(light_dir);
	SetCameraPositionAndTarget_UpVecY(pos_, target_pos_);
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(fov_);

	//リスナーのせってい
	
	SetListener();


}

void Camera::SetListener()
{
	VECTOR dir = VectorAssistant::GetDir(pos_, target_pos_);

	Set3DSoundListenerPosAndFrontPos_UpVecY(pos_, dir);
}

void Camera::Draw()
{
	if (Debug::GetInstance().GetDisp())
	{
		DrawFormatString(100, 120, GetColor(255, 255, 255), "x:%.2f,y:%.2f,z:%.2f", pos_.x, pos_.y, pos_.z);

		DrawSphere3D(target_pos_, 1, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}
}

void Camera::OriginalSetting()
{
	// その前の情報をセッティング
	before_near_				= GetCameraNear();
	before_far_				= GetCameraFar();
	before_fov_				= GetCameraFov();
	before_pos_				= GetCameraPosition();
	before_target_pos_	= GetCameraTarget();

	const VECTOR kInitPos			= VectorAssistant::GetZeroVec();
	const VECTOR kInitTargetPos		= VGet(0.f, 0.f, 10.f);
	const VECTOR kInitLightDir		= VGet(0.f, 0.f, 1.f);
	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);
	//ポジションの指定
	SetCameraPositionAndTarget_UpVecY(kInitPos, kInitTargetPos);
	// 視野角設定
	SetupCamera_Perspective(fov_);
	//ターゲットの方向へのライトを出す
	VECTOR light_dir = VectorAssistant::GetDir(kInitPos, kInitTargetPos);
	SetLightDirection(light_dir);
}

void Camera::BeforeSetting()
{
	SetLightPosition(before_pos_);
	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(before_near_, before_far_);
	//ポジションの指定
	SetCameraPositionAndTarget_UpVecY(before_pos_, before_target_pos_);
	// 視野角設定
	SetupCamera_Perspective(before_fov_);
}
