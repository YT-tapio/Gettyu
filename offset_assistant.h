#pragma once
#include<iostream>
#include"FPS.h"
namespace OffsetAssistant
{
	/// @brief “™‘¬‚Å‘å‚«‚­‚·‚é
	/// @param me 
	/// @param max 
	/// @param speed 
	inline void UniformBig(float& me, const float& max,const float& speed)
	{
		me = me + (speed * FPS::GetInstance().GetDeltaTime());
		me = (me > max) ? max : me;
	}

}