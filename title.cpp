
#include"title.h"
#include"FPS.h"
#include"input.h"
#include"button.h"
#include"button_selecter.h"
#include"keyconfig.h"
#include"camera.h"
#include"color.h"
#include"font.h"
#include"normal_sub_screen.h"
#include"animation.h"
#include"condition_timer.h"
#include"Draw2D.h"

Title::Title(int model)
	:BaseScene(SceneName::kTitle,model)
	,button_num_(0)
{
	const char* kEnemyModelPath		= "data/model/character/enemy/Ch14_nonPBR.mv1";
	enemy_model_							= MV1LoadModel(kEnemyModelPath);

	start_			= FALSE;
	go_input_type_	= FALSE;
	game_end_		= FALSE;

	auto pos_mat	= MGetTranslate(kPos);
	auto scale_mat = MGetScale(kScale);
	auto rot_mat	= MGetRotY(kRotation.y);

	auto enemy_pos_mat = MGetTranslate(kEnemyPos);
	auto enemy_scale_mat = MGetScale(kEnemyScale);
	auto enemy_rot_mat = MGetRotY(kEnemyRotation.y);

	Camera::GetInstance().OriginalSetting();

	animation_				= std::make_shared<Animation>();
	enemy_animation_		= std::make_shared<Animation>();

	mat_				= MMult(MMult(rot_mat,scale_mat), pos_mat);
	enemy_mat_ = MMult(MMult(enemy_rot_mat, enemy_scale_mat), enemy_pos_mat);
	MV1SetMatrix(player_model_, mat_);
	MV1SetMatrix(enemy_model_, enemy_mat_);

	tanuei_font_ = std::make_shared<Font>(kTanueiFontPath, kTanueiFontName, kFontSize, kFontThick,DX_FONTTYPE_EDGE);
	title_ui_screen_ = std::make_shared<NormalSubScreen>(kTitleUiPos, kGameWidth, kGameHeight, kTitleUiWidth, kTitleUiHeight, TRUE, AlphaColorType::kBlack, 10, TRUE);
	title_ui_screen_->SetIsDisp(TRUE);

	transition_timer_ = std::make_shared<ConditionTimer>(5.f);
	fade_in_param_ = 0.f;
}


Title::~Title()
{
	animation_->Detach(AnimationType::kFastRun);
	enemy_animation_->Detach(AnimationType::kFastRun);
	MV1DeleteModel(enemy_model_);
}

void Title::AnimationSetting()
{
	AnimationData dash;
	AnimationData enemy_dash;
	const char kFastRunAnimPath[256]				= "data/animation/Fast_Run.mv1";
	const char kEnemyFastRunAnimPath[256]	= "data/model/character/enemy/animation/Standard_Run.mv1";
	const float kFastRunAnimSpeed					= 3.f;
	const float kEnemyFastRunAnimSpeed			= 5.f;
	
	Load(dash				, kFastRunAnimPath			, AnimationType::kFastRun, player_model_	, 0	, kFastRunAnimSpeed);
	Load(enemy_dash	, kEnemyFastRunAnimPath	, AnimationType::kFastRun, enemy_model_	, 1	, kEnemyFastRunAnimSpeed);

	animation_->Add(dash);
	enemy_animation_->Add(enemy_dash);

	animation_->Attach(AnimationType::kFastRun);
	enemy_animation_->Attach(AnimationType::kFastRun);
}

void Title::Setting()
{
	title_ui_screen_->Up();

	DrawStringToHandle(80
		, 200, "‚°‚Á‚¿‚ã`", Color::kYellow, tanuei_font_->GetHandle(), Color::kRed);

	title_ui_screen_->Down();

	MV1SetMatrix(player_model_, mat_);
}

void Title::FadeOut()
{
	const float kFadeInMax = 255.f;

	if (fade_in_param_ == kFadeInMax) { return; }

	const float kFadeInSpeed = 10.f;
	fade_in_param_ += kFadeInSpeed * FPS::GetInstance().GetDeltaTime();

	fade_in_param_ = (fade_in_param_ > kFadeInMax) ? kFadeInMax : fade_in_param_;
}

void Title::Init()
{
	int button_num = 0;
	SetMouseDispFlag(TRUE);
	selecter_ = std::make_shared<ButtonSelecter>();
	buttons_.push_back(std::make_shared<Button>(kGameStartButtonPos, kButtonWidth, kButtonHeight, "", button_num,&start_));
	button_num++;
	buttons_.push_back(std::make_shared<Button>(kInputTypeButtonPos, kButtonWidth, kButtonHeight, "", button_num, &go_input_type_));
	button_num++;
	buttons_.push_back(std::make_shared<Button>(kGameEndButtonPos, kButtonWidth, kButtonHeight, "", button_num, &game_end_));
	AnimationSetting();
}

void Title::Update(SceneName& name)
{
	animation_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());
	enemy_animation_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());
	Setting();
	animation_->Update(AnimationType::kFastRun);
	enemy_animation_->Update(AnimationType::kFastRun);
	button_num_ = button_num_ + selecter_->Select(SelectType::kVertical);

	if (button_num_ < 0)
	{
		button_num_ = 0;
	}

	if (button_num_ > 2)
	{
		button_num_ = 2;
	}


	

	if (start_)
	{
		transition_timer_->Update();

		FadeOut();

		if (transition_timer_->GetIsEnd())
		{
			name = SceneName::kGame;
		}

	}
	else
	{
		for (auto& button : buttons_)
		{
			button->Update(button_num_);

			if (button->GetState() >= ButtonState::kSelect)
			{
				button_num_ = button->GetNum();
			}

		}
	}

	


	if (game_end_)
	{
		name = SceneName::kEnd;
	}
	
	// name = SceneName::kGame;
}

void Title::Draw()
{
	
	DrawFormatString(20, 20, GetColor(255, 255, 255), "Title");
	DrawFormatString(20, 35, GetColor(255, 255, 255), "SPACE / A Button : game start");
	

	MV1DrawModel(player_model_);
	MV1DrawModel(enemy_model_);
	
	for (const auto& button : buttons_)
	{
		button->Draw();
	}

	title_ui_screen_->Draw();
	
	Draw2D::WhiteBoxBlend(fade_in_param_);
	//title_ui_screen_->Debug();

}


