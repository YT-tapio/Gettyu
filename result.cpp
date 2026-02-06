#include<iostream>
#include<vector>
#include"result.h"
#include"FPS.h"
#include"input.h"
#include"keyconfig.h"
#include"Draw2D.h"
#include"color.h"
#include"button.h"
#include"button_selecter.h"
#include"object_base.h"
#include"character_dance.h"
#include"normal_sub_screen.h"
#include"font.h"
#include"enemy_get_num.h"
#include"sound.h"
#include"2D_sound.h"
#include"result_score.h"
#include"button_graph_create.h"
#include"condition_timer.h"
#include"tutorial.h"
#include"button_decide.h"

Result::Result(int model)
	:BaseScene(SceneName::kResult,model)
{
	const VECTOR kButtonCenterPos = VectorAssistant::Get2DVec(1150.f, 600.f);
	const float kButtonWidth = 200.f;
	const float kButtonHeight = 100.f;


	const VECTOR kEnemyPos			= VGet(0.f, -8.f, 13.f);
	const VECTOR kEnemyRot			= VectorAssistant::GetZeroVec();
	const VECTOR kEnemyScale		= VectorAssistant::GetSame3DVec(0.1f);
	const char* kEnemyPath				= "data/model/character/enemy/Ch14_nonPBR.mv1";
	
	const char* kEnemyAnimPath		= "data/model/character/enemy/animation/Laughing.mv1";
	const float kEnemyAnimSpeed		= 3.f;

	const int kEnemyScreenWidth = 180;
	const int kEnemyScreenHeight = 180;

	//ゲットした敵を横にうつす最大量
	const int kMaxDispWidthNum = 4;

	const VECTOR kEnemyScreenInitPos = VectorAssistant::Get2DVec(620.f, 120.0f);

	int enemy_get_num = EnemyGetNum::GetInstance().GetNum();		//ゲットしたenemyの数を記憶

	int button_num		= 0;
	button_num_			= 0;
	time_					= ClearTime::GetInstance().GetClearTime();

	go_title_	= FALSE;
	restart_		= FALSE;
	tanuei_font_	= std::make_shared<Font>(kTanueiFontPath, kTanueiFontName, kFontSize, kFontThick, DX_FONTTYPE_EDGE);
	animation_	= std::make_shared<Animation>();
	selecter_		= std::make_shared<ButtonSelecter>();

	const char* kBgmPath			= "data/sound/result/bgm/bgm.mp3";
	const char* kSelectSoundPath	= "data/sound/button/select.mp3";
	
	bgm_ = std::make_shared<Sound2D>(kBgmPath, DX_PLAYTYPE_BACK, 100, TRUE);
	select_sound_ = std::make_shared<Sound2D>(kSelectSoundPath, DX_PLAYTYPE_BACK, 80, FALSE);

	next_scene_offset_timer_ = std::make_shared<ConditionTimer>(2.f);

	// ボタンを作る
	auto retry_handle = ButtonGraph::GetInstance().GetRetryHandle();
	auto go_title_handle = ButtonGraph::GetInstance().GetGoTitleHandle();

	buttons_.push_back(std::make_shared<Button>(kButtonCenterPos, kButtonWidth, kButtonHeight, "", button_num, &restart_, retry_handle));
	button_num++;
	buttons_.push_back(std::make_shared<Button>(VAdd(kButtonCenterPos ,VGet(0.f,(kButtonHeight) +10,0.f)), kButtonWidth, kButtonHeight, "", button_num, &go_title_, go_title_handle));
	button_num++;
	AnimationData enemy_anim_data;

	Load(enemy_anim_data, kEnemyAnimPath, AnimationType::kIdle, -1, 1, kEnemyAnimSpeed);

	for (int i = 0; i < enemy_get_num; i++)
	{
		float anim_speed = kEnemyAnimSpeed;
		if (i % 2) { anim_speed += 1.f; }
		enemy_anim_data.play_speed = anim_speed;
		objects_.push_back(std::make_shared<CharacterDance>(kEnemyPos, kEnemyRot, kEnemyScale, kEnemyPath, enemy_anim_data));
	}

	const char* kSkyDomePath	= "data/skydome/Dome_SS601.mv1";
	const float kSkyDomeScale	= 0.8f;

	const char* kStagePath		= "data/model/map/arena/map.mv1";
	const float kStageScale		= 1.f;
	back_objects_.push_back(std::make_shared<ObjectBase>(VectorAssistant::GetZeroVec(), VectorAssistant::GetZeroVec(), VectorAssistant::GetSame3DVec(kSkyDomeScale), kSkyDomePath));
	back_objects_.push_back(std::make_shared<ObjectBase>(VGet(0.f,-10.f,0.f), VectorAssistant::GetZeroVec(), VectorAssistant::GetSame3DVec(kStageScale), kStagePath));
	
	for (int i = 0; i < enemy_get_num; i++)
	{
		int num_x = i % kMaxDispWidthNum;
		int num_y = num_y = i / kMaxDispWidthNum;

		VECTOR screen_pos = VAdd(kEnemyScreenInitPos, VectorAssistant::Get2DVec(kEnemyScreenWidth * num_x, kEnemyScreenHeight * num_y));

		enemy_screens_.push_back(std::make_shared<NormalSubScreen>(screen_pos, kGameWidth, kGameHeight,
			kEnemyScreenWidth, kEnemyScreenHeight, TRUE, AlphaColorType::kBlack, 10, TRUE));
	}


	for (auto& screen : enemy_screens_)
	{
		screen->SetIsDisp(TRUE);
	}
	result_sentence_ = std::make_shared<ResultScoreUI>(time_);
	button_decide_ui_ = std::make_shared<ButtonDecideUI>();
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

void Result::FadeOut()
{
	const float kFadeOutSpeed	= 20.f;
	const float kFadeOutMax		= 255.f;

	fade_out_param_ += kFadeOutSpeed * FPS::GetInstance().GetDeltaTime();

	if (fade_out_param_ > kFadeOutMax)
	{
		fade_out_param_ = kFadeOutMax;
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
	auto rot_mat		= MGetRotY(kRotation.y);
	auto scale_mat	= MGetScale(kScale);
	auto pos_mat		= MGetTranslate(kPos);

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

void Result::UpdateDispEnemyScreen()
{
	//やり方がわからないのでいったんごり押しで
	int screen_num = 0;

	for (auto& screen : enemy_screens_)
	{
		int enemy_num = 0;
		screen->Up();
		screen->SetUpCamera();

		auto light_dir = GetLightDirection();
		SetLightDirection(VGet(0.f, 0.f, 1.f));

		for (auto& obj : objects_)
		{
			if (screen_num == enemy_num) 
			{
				obj->Draw();
				//Draw2D::Box(VectorAssistant::Get2DVec(300.f, 300.f), 100, 100, Color::kRed, TRUE);

				break;
			}
			else
			{
				enemy_num++;
			}
			
		}
		SetLightDirection(light_dir);
		screen->SetUpOrignalCamera();
		screen->Down();
		screen_num++;
	}
}

void Result::Init()
{
	SetMouseDispFlag(TRUE);
	fade_in_param_			= kInitFadeInParamMax;
	fade_out_param_		= 0;
	is_fade_in_ = TRUE;
	
	//アニメーションの適応を行う
	AddAnim();
	animation_->Attach(kAnimType);
}

void Result::Update(SceneName& name)
{
	//name = SceneName::kTitle;

	animation_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());

	for (auto& obj : objects_)
	{
		obj->SetDeltaTime();
		obj->Update();
	}

	for (auto& obj : back_objects_)
	{
		obj->SetDeltaTime();
		obj->Update();
	}

	Setting();
	
	if (Input::GetInstance().GetDeviceType() == InputDeviceType::kPad)
	{
		SetMouseDispFlag(FALSE);
	}
	else
	{
		SetMouseDispFlag(TRUE);
	}

	UpdateDispEnemyScreen();
	if (next_scene_offset_timer_->GetIsEnd()) 
	{
		if (restart_)
		{
			if (Tutorial::GetInstance().GetIsTutorial())
			{
				Tutorial::GetInstance().ChangeTutorial(TRUE);
			}

			name = SceneName::kGame;
		}

		if (go_title_)
		{
			name = SceneName::kTitle;
		}

		return;
	}

	if (go_title_ || restart_)
	{
		next_scene_offset_timer_->Update();
		FadeOut();
		return;
	}

	button_num_ += selecter_->Select(SelectType::kVertical);

	if (button_num_ < 0)
	{
		button_num_ = 1;
	}

	if (button_num_ > 1)
	{
		button_num_ = 0;
	}

	if (before_button_num_ != button_num_)
	{
		select_sound_->Reset();
		select_sound_->Update();
		before_button_num_ = button_num_;
	}

	for (auto& button : buttons_)
	{
		button->Update(button_num_);

		button->Update(button_num_);

		if (button->GetState() >= ButtonState::kSelect)
		{
			button_num_ = button->GetNum();
		}

	}

	result_sentence_->Update();
	bgm_->Update();

	if (is_fade_in_)
	{
		FadeIn();
	}


	

	// playerのモデルにダンスさせる
	

	/// name = SceneName::kTitle;
}

void Result::Draw()
{
	SetUseLighting(FALSE);
	for (auto& obj : back_objects_)
	{
		obj->Draw();
	}
	SetUseLighting(TRUE);

	MV1DrawModel(player_model_);
	

	for (auto& screen : enemy_screens_)
	{
		screen->Draw();
	}

	for (auto& button : buttons_)
	{
		button->Draw();
	}

	result_sentence_->Draw();
	if (!Tutorial::GetInstance().GetIsTutorial())
	{
		DrawFormatStringToHandle(static_cast<int>(kClearTimerPos.x), static_cast<int>(kClearTimerPos.y),
			kFontColor, tanuei_font_->GetHandle(), "%.1f", time_, kFontThickColor);
	}
	
	button_decide_ui_->Draw();

	Draw2D::WhiteBoxBlend(static_cast<int>(fade_in_param_));
	Draw2D::WhiteBoxBlend(static_cast<int>(fade_out_param_));
}