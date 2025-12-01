#pragma once

#include<iostream>
#include"DxLib.h"

namespace Collision2D
{
	/// <summary>
	/// box‚Ì’†‚É‚¢‚é‚©‚Ì”»’è
	/// </summary>
	/// <param name="my_pos">©•ª</param>
	/// <param name="box_center_pos">box‚Ì’†“_</param>
	/// <param name="width">‰¡</param>
	/// <param name="height">c</param>
	/// <returns>’†‚É‚¢‚éFTRUE</returns>
	inline bool IsInBox(const VECTOR my_pos, const VECTOR box_center_pos, int width, int height)
	{
		
		// ‰E
		if (my_pos.x > (box_center_pos.x + float(width) * 0.5))
		{
			return FALSE;
		}
		// ¶
		if (my_pos.x < (box_center_pos.x - float(width) * 0.5))
		{
			return FALSE;
		}
		//ã
		if (my_pos.y < (box_center_pos.y - float(height) * 0.5))
		{
			return FALSE;
		}
		//‰º
		if (my_pos.y > (box_center_pos.y + float(height) * 0.5))
		{
			return FALSE;
		}
		return TRUE;
	}



	

}