#include"result.h"

Result::Result()
	:BaseScene(SceneName::kResult)
{
	
}


Result::~Result()
{

}

void Result::Init()
{

}

void Result::Update(SceneName& name)
{
	if (CheckHitKey(KEY_INPUT_P))
	{
		name = SceneName::kTitle;
	}

	ClearDrawScreen();

	Draw();

	ScreenFlip();

}

void Result::Draw()
{
	DrawFormatString(20, 20, GetColor(255, 255, 255), "Result");
}