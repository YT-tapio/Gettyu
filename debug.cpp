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



void Debug::VectorDraw(const VECTOR& vec)
{
	DrawFormatString(0, Debug::GetInstance().GetFontSize() * Debug::GetInstance().GetCurrentNum(), kDebugColor, "vec : x % .2f,y % .2f,z % .2f", vec.x, vec.y, vec.z);
	Debug::GetInstance().Add();
}
