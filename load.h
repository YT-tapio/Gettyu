#pragma once
#include"vector_assistant.h"

class LoadUI
{
private:

	const VECTOR kPos = VectorAssistant::Get2DVec(1000.f, 750.f);
	const VECTOR kOriginalSize = VectorAssistant::Get2DVec(214.f, 62.f);
	const VECTOR kScale = VectorAssistant::GetSame2DVec(2.f);

	int handle_;

public:

	LoadUI();

	~LoadUI();

	void Draw();

};