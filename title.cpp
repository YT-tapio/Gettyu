
#include"title.h"
#include"input.h"
#include"button.h"
#include"button_selecter.h"
#include"keyconfig.h"
#include"vector_assistant.h"

Title::Title()
	:BaseScene(SceneName::kTitle)
	,button_num_(0)
{
	start_ = FALSE;
	flag1_ = FALSE;
	flag2_ = FALSE;
}


Title::~Title()
{

}

void Title::Init()
{
	selecter_ = std::make_shared<ButtonSelecter>();
	SetMouseDispFlag(TRUE);
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(100.f, 200.f), 100, 100, "", 0,&start_));
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(250.f, 200.f), 100, 100, "", 1, &flag1_));
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(400.f, 200.f), 100, 100, "", 2, &flag2_));
}

void Title::Update(SceneName& name)
{
	//name = SceneName::kGame;
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

	
	

	// name = SceneName::kGame;
}

void Title::Draw()
{
	DrawFormatString(20, 20, GetColor(255, 255, 255), "Title");
	DrawFormatString(20, 35, GetColor(255, 255, 255), "SPACE / A Button : game start");

	for (const auto& button : buttons_)
	{
		button->Draw();
	}

	

}


