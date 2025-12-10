#pragma once
#include"DxLib.h"
#include"screen.h"
#include"color.h"
#include"vector_assistant.h"

namespace Draw2D
{
	// 中心座標の設定
	const VECTOR kCenterPos = VectorAssistant::Get2DVec((kGameWidth * 0.5f), (kGameHeight * 0.5f));

	/// @brief ボックスの描画
	/// @param pos 中心座標
	/// @param width よこ
	/// @param height たて
	/// @param color 色
	/// @param alpha 枠を出すのかどうか , TRUE 全部描画 : FALSE 枠だけ
	inline void Box(const VECTOR& pos, int width, int height, int color,const bool alpha)
	{
		DrawBox(static_cast<int>(pos.x - (float(width) * 0.5f)),
			static_cast<int>(pos.y - (float(height) * 0.5f)),
			static_cast<int>(pos.x + (float(width) * 0.5f)),
			static_cast<int>(pos.y + (float(height) * 0.5f)),
			color, alpha);
	}

	/// <summary>
	/// 2Dの丸を描画
	/// </summary>
	/// <param name="pos">中心座標</param>
	/// <param name="radius">半径</param>
	/// <param name="color">色</param>
	/// <param name="alpha">枠を出すのかどうか , TRUE 全部描画 : FALSE 枠だけ</param>
	inline void Circle(const VECTOR& pos, const float& radius, int color, bool alpha)
	{
		DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), radius, color, alpha);
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

	/// <summary>
	/// 丸をブレンドして描画
	/// </summary>
	/// <param name="pos">中心座標</param>
	/// <param name="radius">半径</param>
	/// <param name="color">色</param>
	/// <param name="alpha">枠を出すのかどうか , TRUE 全部描画 : FALSE 枠だけ</param>
	/// <param name="alpha_num">透過 ; 大きくすると描画されない max 255</param>
	inline void BlendCircle(const VECTOR& pos, const float& radius, int color, bool alpha, const int& alpha_num)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_num);
		Circle(pos, radius, color, alpha);
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
			static_cast<int>(pos.y - (float(height) * 0.5f)),
			static_cast<int>(pos.x + (float(width) * 0.5f)),
			static_cast<int>(pos.y + (float(height) * 0.5f)),
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

	/// @brief 汎用性が高そうなので関数化しておく : 画面全体を覆う,black_outなどを行う
	/// @param alpha_num	透過率
	inline void BlackBoxBlend(const int& alpha_num)
	{
		BlendBox(kCenterPos, kGameWidth, kGameHeight, Color::kBlack, TRUE, alpha_num);
	}

	/// @brief 汎用性が高そうなので関数化しておく : 画面全体を覆う,white_outなどを行う
	/// @param alpha_num	透過率
	inline void WhiteBoxBlend(const int& alpha_num)
	{
		BlendBox(kCenterPos, kGameWidth, kGameHeight, Color::kWhite, TRUE, alpha_num);
	}
}
