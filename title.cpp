#include"title.h"

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
	if (CheckHitKey(KEY_INPUT_TAB))
	{
		name = SceneName::kGame;
	}
}

void Title::Draw()
{
	DrawFormatString(20, 20, GetColor(255, 255, 255), "Title");
}


