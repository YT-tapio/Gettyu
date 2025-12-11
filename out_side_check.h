#pragma once
#include"DxLib.h"
#include"vector_assistant.h"

/// @brief 場外のときの判断をしてくれる関数軍
namespace OutSide
{
	/// @brief 場外の判定を行います
	/// @param pos 
	/// @return 
	inline bool Check(const VECTOR& pos)
	{
		// 場外の指定
		// y座標が低すぎるとき

		const float kMinY = -150.f;

		bool flag = (kMinY > pos.y);

		return flag;
	}

	/// @brief 一番近いリスポーンの場所を返してくれる
	/// @param pos 
	/// @return 
	inline VECTOR MostNearRespawn(const VECTOR& pos)
	{
		const int kRespawnPosNum = 4;
		VECTOR respawn_pos[kRespawnPosNum];

		respawn_pos[0] = VectorAssistant::GetZeroVec();
		respawn_pos[1] = VGet(-1.5f, 7.6f, -162.0);
		respawn_pos[2] = VGet(-1.9f, 8.2f, -430.f);
		respawn_pos[3] = VGet(-1.4f, -1.5f, -813.9f);

		// 一番近い物を選ぶ

		VECTOR near_pos = VectorAssistant::GetZeroVec();
		float near_size = 0.f;

		for (int i = 0; i < kRespawnPosNum; i++)
		{
			if (i == 0)
			{
				near_pos = respawn_pos[i];
				near_size = VectorAssistant::GetDistSize(pos,respawn_pos[i]);
			}
			else
			{
				float size = VectorAssistant::GetDistSize(pos, respawn_pos[i]);

				if (near_size > size)
				{
					near_size = size;
					near_pos = respawn_pos[i];
				}
			}
		}

		return near_pos;
	}


}

