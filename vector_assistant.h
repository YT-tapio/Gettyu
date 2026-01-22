#pragma once
#include<math.h>
#include"DxLib.h"
#include"screen.h"
#include"const_rad.h"

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

	inline VECTOR GetScreenCenterPos()
	{
		float x = kGameWidth * 0.5f;
		float y = kGameHeight * 0.5f;
		return Get2DVec(x, y);
	}


	inline VECTOR GetReverce(const VECTOR& vec)
	{
		const float kReverceNum = -1.f;
		return VScale(vec, kReverceNum);
	}

	/// <summary>
	/// xyz‚ª“¯‚¶‚Ìvec‚ğ•Ô‚·
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	inline VECTOR GetSame3DVec(const float& num)
	{
		return VGet(num, num, num);
	}

	/// <summary>
	/// xy‚ª“¯‚¶‚Ìvec‚ğ•Ô‚·
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	inline VECTOR GetSame2DVec(const float& num)
	{
		return VGet(num, num, 0.f);
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

	/// <summary>
	/// x²‰ñ“]‚³‚¹‚½‚Ìvector(ƒ‰ƒWƒAƒ“Šp‚Å‚Í‚È‚­À”’l)
	/// </summary>
	/// <param name="me"></param>
	/// <param name="num">(-3.14`3.14)</param>
	/// <returns></returns>
	inline VECTOR VGetRotPiX(const VECTOR& me, const float num)
	{
		VECTOR value = GetZeroVec();

		value.x = me.x;
		value.y = (me.y * cosf(num)) - (me.z * sinf(num));
		value.z = (me.y * sinf(num)) + (me.x * cosf(num));

		return value;
	}

	/// <summary>
	/// x²‰ñ“]‚µ‚½‚Æ‚«‚Ìvector
	/// </summary>
	/// <param name="me">q‚ÌƒxƒNƒgƒ‹‚ğ‰ñ“]</param>
	/// <param name="rad">ƒ‰ƒWƒAƒ“Šp(-180`180)</param>
	/// <returns></returns>
	inline VECTOR VGetRotRadX(const VECTOR& me, const float rad)
	{
		return VGetRotPiX(me, kOneRad * rad);
	}

	/// <summary>
	/// y²‰ñ“]‚³‚¹‚½‚Ìvector(ƒ‰ƒWƒAƒ“Šp‚Å‚Í‚È‚­À”’l)
	/// </summary>
	/// <param name="me"></param>
	/// <param name="num">(-3.14`3.14)</param>
	/// <returns></returns>
	inline VECTOR VGetRotPiY(const VECTOR& me, const float num)
	{
		VECTOR value = GetZeroVec();

		value.x = (me.x * cosf(num)) + (me.z * sinf(num));
		value.y = me.y;
		value.z = (-me.x * sinf(num)) + (me.z * cosf(num));

		return value;
	}

	/// <summary>
	/// y²‰ñ“]‚µ‚½‚Æ‚«‚Ìvector
	/// </summary>
	/// <param name="me">q‚ÌƒxƒNƒgƒ‹‚ğ‰ñ“]</param>
	/// <param name="rad">ƒ‰ƒWƒAƒ“Šp(-180`180)</param>
	/// <returns></returns>
	inline VECTOR VGetRotRadY(const VECTOR& me, const float rad)
	{
		return VGetRotPiY(me, kOneRad * rad);
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
