
#include"title.h"
#include"input.h"
#include"button.h"
#include"keyconfig.h"
#include"vector_assistant.h"

Title::Title()
	:BaseScene(SceneName::kTitle)
	,button_num_(0)
{
	
}


Title::~Title()
{

}

void Title::Init()
{
	SetMouseDispFlag(TRUE);
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(100.f, 200.f), 100, 100, "", 0));
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(250.f, 200.f), 100, 100, "", 1));
	buttons_.push_back(std::make_shared<Button>(VectorAssistant::Get2DVec(400.f, 200.f), 100, 100, "", 2));
}

void Title::Update(SceneName& name)
{
	if (Input::GetInstance().CheckInputKey(KeyConfig::kChangeSceneKey) == InputState::kPush || 
		Input::GetInstance().CheckInputPadButton(PadConfig::kChangeSceneButton) == InputState::kPush)
	{
		name = SceneName::kGame;
	}

	for (auto& button : buttons_)
	{
		button->Update(button_num_);

		if (button->GetState() == ButtonState::kSelect)
		{
			button_num_ = button->GetNum();
		}

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


