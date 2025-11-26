#pragma once
#include<math.h>
#include"DxLib.h"


namespace VectorAssistant
{

	inline VECTOR GetZeroVec()
	{
		return VGet(0.f, 0.f, 0.f);
	}

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

	/// <summary>
	/// ³Ë‰evec1‚É‰f‚évec2‚Ì‰e
	/// </summary>
	/// <param name="vec1"></param>
	/// <param name="vec2"></param>
	/// <returns></returns>
	inline VECTOR GetProj(const VECTOR& vec1, const VECTOR& vec2)
	{
		VECTOR proj = VGet(0.f, 0.f, 0.f);

		//•ª•ê
		float denominator = 0.f;

		//vector‚ÌƒTƒCƒY‚ğó‚¯æ‚é

		float vec_size = VSize(vec1);

		denominator = vec_size * vec_size;

		//•ªq
		float molecule;

		molecule = VDot(vec1, vec2);

		float num = (molecule / denominator);

		proj = VScale(vec1, num);


		return proj;


	}

}
