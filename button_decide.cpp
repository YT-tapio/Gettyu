#include"DxLib.h"
#include"button_decide.h"
#include"Draw2D.h"

ButtonDecideUI::ButtonDecideUI()
{
	const char* kButtonPath = "data/image/pad/a_button.png";
	const char* kDecidePath = "data/image/decide_info.png";

	button_handle_ = LoadGraph(kButtonPath);
	decide_handle_ = LoadGraph(kDecidePath);
}

ButtonDecideUI::~ButtonDecideUI()
{
	DeleteGraph(button_handle_);
	DeleteGraph(decide_handle_);
}

void ButtonDecideUI::Update()
{

}

void ButtonDecideUI::Draw()
{
	Draw2D::ExtendGraph(kButtonPos, static_cast<int>(kOriginalButtonSize.x * kButtonScale.x), static_cast<int>(kOriginalButtonSize.y * kButtonScale.y), button_handle_, TRUE);
	Draw2D::ExtendGraph(kDecidePos, static_cast<int>(kOriginalDecideSize.x * kDecideScale.x), static_cast<int>(kOriginalDecideSize.y * kDecideScale.y), decide_handle_, TRUE);
}
