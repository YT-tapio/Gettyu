#include"debug.h"
#include"input.h"
#include"keyconfig.h"

void Debug::CheckChangeDisp()
{
	if (Input::GetInstance().CheckInputKey(KeyConfig::kChageDebugKey) == InputState::kPush ||
		Input::GetInstance().CheckInputPadButton(PadConfig::kChageDebugButton) == InputState::kPush)
	{
		disp_ = !disp_;
	}
}
