#pragma once
#include"DxLib.h"

namespace Draw2D
{
	/// @brief ボックスの描画
	/// @param pos 中心座標
	/// @param width よこ
	/// @param height たて
	/// @param color 色
	/// @param alpha 枠を出すのかどうか , TRUE 全部描画 : FALSE 枠だけ
	inline void Box(const VECTOR& pos, int width, int height, int color,const bool alpha)
	{
		DrawBox(static_cast<int>(pos.x - (float(width) * 0.5f)),
			static_cast<int>(pos.y - (float(width) * 0.5f)),
			static_cast<int>(pos.x + (float(width) * 0.5f)),
			static_cast<int>(pos.y + (float(width) * 0.5f)),
			color, alpha);
	}

	/// <summary>
	/// boxの透過を行う
	/// </summary>
	/// <param name="pos">中心座標</param>
	/// <param name="width">よこ</param>
	/// <param name="height">たて</param>
	/// <param name="color">色</param>
	/// <param name="alpha">枠を出すのかどうか , TRUE 全部描画 : FALSE 枠だけ</param>
	/// <param name="alpha_num">透過 ; 大きくすると描画されない max 255</param>
	inline void BlendBox(const VECTOR& pos, int width, int height, int color, const bool& alpha, const int& alpha_num)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_num);
		Box(pos, width, height, color, alpha);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/// @brief 調整した画像を描画
	/// @param pos 中心座標
	/// @param width よこ
	/// @param height たて
	/// @param data 画像データ
	/// @param alpha 透過するか TRUE 透過
	inline void ExtendGraph(const VECTOR& pos, int width, int height,const int& data, const bool alpha)
	{

		DrawExtendGraph(static_cast<int>(pos.x - (float(width) * 0.5f)),
			static_cast<int>(pos.y - (float(width) * 0.5f)),
			static_cast<int>(pos.x - (float(width) * 0.5f)),
			static_cast<int>(pos.y + (float(width) * 0.5f)),
			data, alpha);

	}


	/// @brief 画像の透過を行う
	/// @param pos 中心の座標
	/// @param width よこ
	/// @param height たて
	/// @param data 画像データ
	/// @param alpha 透過するか TRUE 透過
	/// @param alpha_num 透過率
	inline void BlendGraph(const VECTOR& pos, int width, int height, const int& data, const bool alpha, const int& alpha_num)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_num);
		ExtendGraph(pos, width, height, data, alpha);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	

}
