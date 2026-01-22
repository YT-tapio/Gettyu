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
	/// xyzが同じのvecを返す
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	inline VECTOR GetSame3DVec(const float& num)
	{
		return VGet(num, num, num);
	}

	/// <summary>
	/// xyが同じのvecを返す
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	inline VECTOR GetSame2DVec(const float& num)
	{
		return VGet(num, num, 0.f);
	}

	/// @brief yを無視したvector
	/// @param  
	/// @return 
	inline VECTOR GetPlane(const VECTOR& vec)
	{
		return VGet(vec.x, 0.f, vec.z);
	}

	/// @brief 引数のdirを返す
	/// @param me 自分
	/// @param other 他
	/// @return 正規化されたdirを返す
	inline VECTOR GetDir(const VECTOR& me, const VECTOR& other)
	{
		return VNorm(VSub(other, me));
	}

	/// @brief dirからrad角度を出す
	/// @param dir 
	/// @return 
	inline float GetPlaneRad(const VECTOR& dir)
	{
		return atan2f(dir.x, dir.z);
	}

	/// @brief 自分の半分を出す
	/// @param pos 
	/// @return 
	inline VECTOR GetHerf(const VECTOR& pos)
	{
		return VScale(pos, 0.5f);
	}

	/// <summary>
	/// x軸回転させた時のvector(ラジアン角ではなく実数値)
	/// </summary>
	/// <param name="me"></param>
	/// <param name="num">(-3.14～3.14)</param>
	/// <returns></returns>
	inline VECTOR VGetRotPiX(const VECTOR& me, const float num)
	{
		VECTOR value = GetZeroVec();

		value.x = me.x;
		value.y = (me.y * cosf(num)) - (me.z * sinf(num));
		value.z = (me.y * sinf(num)) + (me.z * cosf(num));

		return value;
	}

	/// <summary>
	/// x軸回転したときのvector
	/// </summary>
	/// <param name="me">子のベクトルを回転</param>
	/// <param name="rad">ラジアン角(-180～180)</param>
	/// <returns></returns>
	inline VECTOR VGetRotRadX(const VECTOR& me, const float rad)
	{
		return VGetRotPiX(me, kOneRad * rad);
	}

	/// <summary>
	/// y軸回転させた時のvector(ラジアン角ではなく実数値)
	/// </summary>
	/// <param name="me"></param>
	/// <param name="num">(-3.14～3.14)</param>
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
	/// y軸回転したときのvector
	/// </summary>
	/// <param name="me">子のベクトルを回転</param>
	/// <param name="rad">ラジアン角(-180～180)</param>
	/// <returns></returns>
	inline VECTOR VGetRotRadY(const VECTOR& me, const float rad)
	{
		return VGetRotPiY(me, kOneRad * rad);
	}

	/// <summary>
	/// x軸回転させた時のvector(ラジアン角ではなく実数値)
	/// </summary>
	/// <param name="me">変換したいベクトル</param>
	/// <param name="num">(-3.14～3.14)</param>
	/// <returns></returns>
	inline VECTOR VGetRotPiZ(const VECTOR& me, const float num)
	{
		VECTOR value = GetZeroVec();

		value.x = (me.x * cosf(num)) - (me.y * sinf(num));
		value.y = (me.x * sinf(num)) + (me.y * cosf(num));
		value.z = me.z;

		return value;
	}

	/// <summary>
	/// x軸回転したときのvector
	/// </summary>
	/// <param name="me">変換したいベクトル</param>
	/// <param name="rad">ラジアン角(-180～180)</param>
	/// <returns></returns>
	inline VECTOR VGetRotRadZ(const VECTOR& me, const float rad)
	{
		return VGetRotPiZ(me, kOneRad * rad);
	}

	/// @brief y軸の回転量を返す
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

	/// @brief 自身のposからほかのposまでの距離を取る
	/// @param me 自分
	/// @param other 他
	/// @return 距離
	inline float GetDistSize(const VECTOR& me, const VECTOR& other)
	{
		return VSize(VSub(me, other));
	}

	/// <summary>
	/// 正射影vec1に映るvec2の影
	/// </summary>
	/// <param name="vec1"></param>
	/// <param name="vec2"></param>
	/// <returns></returns>
	inline VECTOR GetProj(const VECTOR& vec1, const VECTOR& vec2)
	{
		VECTOR proj = VGet(0.f, 0.f, 0.f);

		//分母
		float denominator = 0.f;

		//vectorのサイズを受け取る

		float vec_size = VSize(vec1);

		denominator = vec_size * vec_size;

		//分子
		float molecule;

		molecule = VDot(vec1, vec2);

		float num = (molecule / denominator);

		proj = VScale(vec1, num);


		return proj;


	}

}
