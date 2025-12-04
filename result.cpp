#include"result.h"
#include"input.h"
#include"keyconfig.h"
Result::Result()
	:BaseScene(SceneName::kResult)
{
	
}


Result::~Result()
{

}

void Result::Init()
{
	SetMouseDispFlag(TRUE);
}

void Result::Update(SceneName& name)
{
	//name = SceneName::kTitle;
	if (Input::GetInstance().CheckInputKey(KeyConfig::kChangeSceneKey) == InputState::kPush ||
		Input::GetInstance().CheckInputPadButton(PadConfig::kChangeSceneButton) == InputState::kPush)
	{
		name = SceneName::kTitle;
	}

	/// name = SceneName::kTitle;
}

void Result::Draw()
{
	DrawFormatString(20, 20, GetColor(255, 255, 255), "Result");
	DrawFormatString(20, 35, GetColor(255, 255, 255), "SPACE / A Button : Title");
}