#include"title.h"
#include"input.h"
#include"keyconfig.h"
Title::Title()
	:BaseScene(SceneName::kTitle)
{
	
}


Title::~Title()
{

}

void Title::Init()
{

}

void Title::Update(SceneName& name)
{
	if (Input::GetInstance().CheckInputKey(KeyConfig::kChangeSceneKey) == InputState::kPush || 
		Input::GetInstance().CheckInputPadButton(PadConfig::kChangeSceneButton) == InputState::kPush)
	{
		name = SceneName::kGame;
	}
}

void Title::Draw()
{
	DrawFormatString(20, 20, GetColor(255, 255, 255), "Title");
	DrawFormatString(20, 35, GetColor(255, 255, 255), "SPACE / A Button : game start");
}


