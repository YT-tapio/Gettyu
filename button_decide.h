#pragma once
#include"vector_assistant.h"

class ButtonDecideUI
{
private:

	const VECTOR kButtonPos		= VectorAssistant::Get2DVec(1100.f, 800.f);
	const VECTOR kDecidePos		= VAdd(kButtonPos, VGet(90.f, 0.f, 0.f));

	const VECTOR kOriginalButtonSize = VectorAssistant::Get2DVec(175.f,122.f);
	const VECTOR kOriginalDecideSize = VectorAssistant::Get2DVec(200.f, 83.f);

	const VECTOR kButtonScale = VectorAssistant::GetSame2DVec(0.5f);
	const VECTOR kDecideScale = VectorAssistant::GetSame2DVec(0.8f);

	int button_handle_;
	int decide_handle_;

public:

	ButtonDecideUI();

	~ButtonDecideUI();

	void Update();

	void Draw();

};