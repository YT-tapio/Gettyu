#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include"rot_function.h"

void CheckReverseRotFunc(float& now_rot, float target_rot,float delta_time,float speed)
{
	//同じときは早期リターン
	if (now_rot == target_rot) { return; }

	//ここで180の値を宣言
	float simple_reverse_num = (static_cast<float>(M_PI / 180) * 180);

	/*---------------------------新しい処理----------------------------*/

	// まずは今の座標から目標の座標までの距離を求める
	// その距離が180度を越えるような大きさだと例外の処理を進める

	// 今からからターゲットまでの回転の距離
	float rot_distance = 0.0f;

	//回転量
	float rot_num = (static_cast<float>((M_PI / 180) * speed)) * (delta_time * 10);

	// 同じときは先にはじくようにしているので大丈夫
	// どちらが小さいかを見て小さいほうから大きいほうを引く
	if (now_rot < target_rot)
	{
		rot_distance = now_rot - target_rot;
	}
	else
	{
		rot_distance = target_rot - now_rot;
	}

	// rot_distanceの絶対値が180より大きいなら
	if (fabs(rot_distance) > simple_reverse_num)
	{
		//現在の回転量がマイナスなら
		if (now_rot < static_cast<float>((M_PI / 180) * 0))
		{
			now_rot -= rot_num;

			//-180を超えるとき
			if (now_rot < -simple_reverse_num)
			{
				//-180からどんだけ超えているのかを確認
				float over_num = now_rot + simple_reverse_num;

				//超過したときの+の値を代入
				now_rot = simple_reverse_num + over_num;

				if (now_rot < target_rot)
				{
					now_rot = target_rot;
				}

			}

		}
		else  //+なら
		{
			now_rot += rot_num;

			//180を超えるとき
			if (now_rot > simple_reverse_num)
			{
				//180からどんだけ超えているかを確認
				float over_num = now_rot - simple_reverse_num;

				//超過したときの-の値を代入
				now_rot = -simple_reverse_num + over_num;

				if (now_rot > target_rot)
				{
					now_rot = target_rot;
				}

			}
		}
	}
	else  //普通の処理
	{
		if (now_rot < target_rot)
		{
			now_rot += rot_num;

			if (now_rot > target_rot)
			{
				now_rot = target_rot;
			}
		}
		else
		{
			now_rot -= rot_num;

			if (now_rot < target_rot)
			{
				now_rot = target_rot;
			}
		}
	}
}