
#include"title.h"
#include"input.h"
#include"button.h"
#include"button_selecter.h"
#include"keyconfig.h"
#include"camera.h"
#include"color.h"
#include"font.h"
#include"normal_sub_screen.h"

Title::Title(int model)
	:BaseScene(SceneName::kTitle,model)
	,button_num_(0)
{
	start_			= FALSE;
	go_input_type_	= FALSE;
	game_end_		= FALSE;

	auto pos_mat	= MGetTranslate(kPos);
	auto rot_mat	= MGetRotY(kRotation.y);
	auto scale_mat	= MGetScale(kScale);

	Camera::GetInstance().OriginalSetting();

	mat_ = MMult(MMult(rot_mat,scale_mat), pos_mat);
	MV1SetMatrix(player_model_, mat_);

	tanuei_font_ = std::make_shared<Font>(kTanueiFontPath, kTanueiFontName, kFontSize, kFontThick,DX_FONTTYPE_EDGE);
	title_ui_screen_ = std::make_shared<NormalSubScreen>(VectorAssistant::Get2DVec(800, 200), kGameWidth, kGameHeight, 300, 300, TRUE, AlphaColorType::kBlack, 10, TRUE);
	title_ui_screen_->SetIsDisp(TRUE);
}


Title::~Title()
{

}

void Title::Setting()
{
	title_ui_screen_->Up();

	DrawStringToHandle(100
		, 100, "‚í‚Á‚µ‚å‚¢", Color::kYellow, tanuei_font_->GetHandle(), Color::kRed);

	title_ui_screen_->Down();

	MV1SetMatrix(player_model_, mat_);
}

void Title::Init()
{
	SetMouseDispFlag(TRUE);
	selecter_ = std::make_shared<ButtonSelecter>();
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(100.f, 200.f), 100, 100, "", 0,&start_));
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(250.f, 200.f), 100, 100, "", 1, &go_input_type_));
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(400.f, 200.f), 100, 100, "", 2, &game_end_));

	
}

void Title::Update(SceneName& name)
{
	
	Setting();

	if (Input::GetInstance().CheckInputKey(KeyConfig::kChangeSceneKey) == InputState::kPush || 
		Input::GetInstance().CheckInputPadButton(PadConfig::kChangeSceneButton) == InputState::kPush)
	{
		name = SceneName::kGame;
	}

	button_num_ = button_num_ + selecter_->Select(SelectType::kSide);

	if (button_num_ < 0)
	{
		button_num_ = 0;
	}

	if (button_num_ > 2)
	{
		button_num_ = 2;
	}

	for (auto& button : buttons_)
	{
		button->Update(button_num_);

		if (button->GetState() >= ButtonState::kSelect)
		{
			button_num_ = button->GetNum();
		}

	}

	if (start_)
	{
		name = SceneName::kGame;
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
	title_ui_screen_->Draw();
	title_ui_screen_->Debug();
	DrawSphere3D(kPos, 3.f, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	MV1DrawModel(player_model_);

	for (const auto& button : buttons_)
	{
		button->Draw();
	}

	

}


