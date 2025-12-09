#include<iostream>
#include<vector>
#include<memory>
#include"result.h"
#include"FPS.h"
#include"input.h"
#include"keyconfig.h"
#include"Draw2D.h"
#include"color.h"
#include"button.h"
#include"button_selecter.h"
//#include"clear_time.h"

Result::Result(int model)
	:BaseScene(SceneName::kResult,model)
{
	const VECTOR kButtonCenterPos = VectorAssistant::Get2DVec(800.f, 700.f);
	const float kButtonWidth = 200;
	const float kButtonHeight = 100;

	int button_num = 0;
	button_num_ = 0;
	time_ = ClearTime::GetInstance().GetClearTime();

	go_title_ = FALSE;

	animation_ = std::make_shared<Animation>();
	selecter_ = std::make_shared<ButtonSelecter>();

	// ボタンを作る
	buttons_.push_back(std::make_shared<Button>(kButtonCenterPos, kButtonWidth, kButtonHeight, "", button_num, &go_title_));
}


Result::~Result()
{
	// アニメーションのでタッチ
	animation_->Detach(kAnimType);
}

void Result::FadeIn()
{
	const float kFadeInSpeed = 6.f;
	const float kFadeInMin = 0.f;

	fade_in_param_ -= kFadeInSpeed * FPS::GetInstance().GetDeltaTime();
	

	if (fade_in_param_ < kFadeInMin)
	{
		fade_in_param_ = kFadeInMin;
		//bool更新：FadeInの終了
		is_fade_in_ = FALSE;
	}
}

void Result::Setting()
{
	
	animation_->Update(kAnimType);

	//奥行1.0～1000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);
	// 視野角設定
	SetupCamera_Perspective(kFov);

	//カメラを設定
	SetCameraPositionAndTarget_UpVecY(kCameraPos, kTargetPos);

	auto dir = VectorAssistant::GetDir(kCameraPos, kTargetPos);

	SetLightDirection(dir);
	SetLightPosition(kCameraPos);

	//modelのset
	auto rot_mat = MGetRotY(kRotation.y);
	auto scale_mat = MGetScale(kScale);
	auto pos_mat = MGetTranslate(kPos);

	mat_ = MMult(MMult(rot_mat, scale_mat), pos_mat);

	MV1SetMatrix(player_model_, mat_);

	

}

void Result::AddAnim()
{
	const float kIdleSpeed = 2.2f;

	AnimationData idle;
	char kIdleAnimationPath[256]  = "data/animation/Zombie_Idle.mv1";

	Load(idle, kIdleAnimationPath, kAnimType, player_model_, 0, kIdleSpeed);

	animation_->Add(idle);

}

void Result::Init()
{
	SetMouseDispFlag(TRUE);
	fade_in_param_ = kInitFadeInParamMax;
	is_fade_in_ = TRUE;
	
	//アニメーションの適応を行う
	AddAnim();
	animation_->Attach(kAnimType);
}

void Result::Update(SceneName& name)
{
	//name = SceneName::kTitle;

	animation_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());

	Setting();

	button_num_ += selecter_->Select(SelectType::kSide);

	if (button_num_ < 0 || button_num_ > 0)
	{
		button_num_ = 0;
	}

	for (auto& button : buttons_)
	{
		button->Update(button_num_);
	}

	
	if (is_fade_in_)
	{
		FadeIn();
	}
	

	if (go_title_)
	{
		name = SceneName::kTitle;
	}

	// playerのモデルにダンスさせる
	

	/// name = SceneName::kTitle;
}

void Result::Draw()
{
	MV1DrawModel(player_model_);
	
	for (auto& button : buttons_)
	{
		button->Draw();
	}

	DrawFormatString(600, 600, GetColor(255, 255, 255), "%.2f",time_);
	// DrawFormatString(20, 20, GetColor(255, 255, 255), "Result");
	// DrawFormatString(20, 35, GetColor(255, 255, 255), "SPACE / A Button : Title");
	Draw2D::WhiteBoxBlend(static_cast<int>(fade_in_param_));
}