#pragma once
#include"DxLib.h"

namespace VectorAssistant
{
	/// @brief y‚ğ–³‹‚µ‚½vector
	/// @param  
	/// @return 
	VECTOR GetPlane(const VECTOR& vec)
	{
		return VGet(vec.x, 0.f, vec.z);
	}

	/// @brief ˆø”‚Ìdir‚ğ•Ô‚·
	/// @param me ©•ª
	/// @param other ‘¼
	/// @return ³‹K‰»‚³‚ê‚½dir‚ğ•Ô‚·
	VECTOR GetDir(const VECTOR& me, const VECTOR& other)
	{
		return VNorm(VSub(other, me));
	}

}
