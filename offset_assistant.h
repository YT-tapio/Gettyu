#pragma once
#include<iostream>
#include"FPS.h"
namespace OffsetAssistant
{

	inline void Big(int& me, const int& max, const float& speed)
	{
		if (me == max) { return; }

		me = static_cast<float>(me) + speed;
		me = (me > max) ? max : me;
	}

	/// @brief 等速で大きくする
	/// @param me 
	/// @param max 
	/// @param speed デルタタイムをかけた
	inline void Bigf(float& me, const float& max,const float& speed)
	{
		if (me == max) { return; }

		me = me + speed;
		//自分よりも大きい時
		if (me > max)
		{
			me = max;
		}
	}

	inline void Small(int& me, const int& min, const float& speed)
	{
		if (me == min) { return; }

		me = static_cast<float>(me) - speed;
		me = (me < min) ? min : me;
	}

	inline void Smallf(float& me, const float& min, const float& speed)
	{
		if (me == min) { return; }

		me = me - speed;
		me = (me < min) ? min : me;
	}

}