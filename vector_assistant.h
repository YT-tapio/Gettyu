#pragma once
#include<math.h>
#include"DxLib.h"


namespace VectorAssistant
{

	inline VECTOR GetZeroVec()
	{
		return VGet(0.f, 0.f, 0.f);
	}


	inline VECTOR Get2DVec(const float& x, const float& y)
	{
		return VGet(x, y, 0.f);
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

	/// @brief dir‚©‚çradŠp“x‚ğo‚·
	/// @param dir 
	/// @return 
	inline float GetPlaneRad(const VECTOR& dir)
	{
		return atan2f(dir.x, dir.z);
	}

	/// @brief ©•ª‚Ì”¼•ª‚ğo‚·
	/// @param pos 
	/// @return 
	inline VECTOR GetHerf(const VECTOR& pos)
	{
		return VScale(pos, 0.5f);
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

	/// @brief ©g‚Ìpos‚©‚ç‚Ù‚©‚Ìpos‚Ü‚Å‚Ì‹——£‚ğæ‚é
	/// @param me ©•ª
	/// @param other ‘¼
	/// @return ‹——£
	inline float GetDistSize(const VECTOR& me, const VECTOR& other)
	{
		return VSize(VSub(me, other));
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
