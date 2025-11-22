#pragma once
#include<math.h>
#include"DxLib.h"


namespace VectorAssistant
{
	/// @brief y‚ğ–³‹‚µ‚½vector
	/// @param  
	/// @return 
	inline VECTOR GetPlane(const VECTOR& vec)
	{
		return VGet(vec.x, 0.f, vec.z);
	}

	/// @brief ˆø”‚Ìdir‚ğ•Ô‚·
	/// @param me ©•ª
	/// @param other ‘¼
	/// @return ³‹K‰»‚³‚ê‚½dir‚ğ•Ô‚·
	inline VECTOR GetDir(const VECTOR& me, const VECTOR& other)
	{
		return VNorm(VSub(other, me));
	}

	/// @brief y²‚Ì‰ñ“]—Ê‚ğ•Ô‚·
	/// @param dir 
	/// @return 
	inline float GetPlaneRot(const VECTOR& dir)
	{
		if (dir.x == 0.f)
		{
			return 0.f;
		}

		return atan2f(dir.x, dir.z);
	}

}
