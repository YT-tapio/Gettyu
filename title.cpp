#include<iostream>
#include"title.h"
#include"input.h"
#include"button.h"
#include"keyconfig.h"
#include"vector_assistant.h"

Title::Title()
	:BaseScene(SceneName::kTitle)
{
	
}


Title::~Title()
{

}

void Title::Init()
{
	SetMouseDispFlag(TRUE);
	button_ = std::make_shared<Button>(VectorAssistant::Get2DVec(100.f, 200.f), 100, 100, "", 0);
}

void Title::Update(SceneName& name)
{
	if (Input::GetInstance().CheckInputKey(KeyConfig::kChangeSceneKey) == InputState::kPush || 
		Input::GetInstance().CheckInputPadButton(PadConfig::kChangeSceneButton) == InputState::kPush)
	{
		name = SceneName::kGame;
	}
	button_->Update(1);

	// name = SceneName::kGame;
}

void Title::Draw()
{
	DrawFormatString(20, 20, GetColor(255, 255, 255), "Title");
	DrawFormatString(20, 35, GetColor(255, 255, 255), "SPACE / A Button : game start");

	button_->Draw();

}


