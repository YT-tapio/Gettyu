#pragma once
#include<iostream>
#include"FPS.h"
namespace OffsetAssistant
{

	inline void UniformBig(int& me, const int& max, const float& speed)
	{
		me = static_cast<float>(me) + (speed * FPS::GetInstance().GetDeltaTime());
		me = (me > max) ? max : me;
	}

	/// @brief “™‘¬‚Å‘å‚«‚­‚·‚é
	/// @param me 
	/// @param max 
	/// @param speed 
	inline void UniformBigf(float& me, const float& max,const float& speed)
	{
		if (me == max) { return; }

		me = me + (speed * FPS::GetInstance().GetDeltaTime());
		//Ž©•ª‚æ‚è‚à‘å‚«‚¢Žž
		if (me > max)
		{
			me = max;
		}
	}

}