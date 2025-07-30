#include<iostream>
#include"DxLib.h"
#include"Player.h"
#include"base_object.h"
#include"stage.h"

Stage::Stage(int model_handle, VECTOR pos, float scale)
	:BaseObject(pos, model_handle)
	, scale_(VGet(scale,scale,scale))
	, wall_num_(0)
	, floor_num_(0)
	, wall_{ nullptr }
	, floor_{ nullptr }
{

}


Stage::~Stage()
{

}


/*------------private------------*/

void Stage::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hit_dim, const VECTOR& check_position)
{
	// 壁ポリゴンと床ポリゴンの数を初期化する
	wall_num_ = 0;
	floor_num_ = 0;

	// 検出されたポリゴンの数だけ繰り返し
	for (int i = 0; i < hit_dim.HitNum; i++)
	{
		// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
		if (hit_dim.Dim[i].Normal.y < 0.000001f && hit_dim.Dim[i].Normal.y >-0.000001f)
		{
			// 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
			if (hit_dim.Dim[i].Position[0].y > check_position.y + 1.0f ||
				hit_dim.Dim[i].Position[1].y > check_position.y + 1.0f ||
				hit_dim.Dim[i].Position[2].y > check_position.y + 1.0f)
			{
				// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if (wall_num_ < kMaxHitColl)
				{
					// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
					wall_[wall_num_] = &hit_dim.Dim[i];
					wall_num_++;
				}
			}

		}
		else
		{
			// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
			if (floor_num_ < kMaxHitColl)
			{
				// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
				floor_[floor_num_] = &hit_dim.Dim[i];
				floor_num_++;
			}
		}
	}
}


VECTOR Stage::CheckHitWithWall(Player& player, const VECTOR& check_position)
{
	VECTOR fixed_pos = check_position;

	// 壁の数が無かったら早期リターン
	if (wall_num_ == 0)
	{
		return fixed_pos;
	}

	// 壁からの押し出し処理を試みる最大数だけ繰り返し
	for (int k = 0; k < kHitTryNum; k++)
	{

		// 当たる可能性のある壁ポリゴンを全て見る
		bool isHitWall = false;
		for (int i = 0; i < wall_num_; i++)
		{
			// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
			auto poly = wall_[i];

			// プレイヤーと当たっているなら
			if (HitCheck_Capsule_Triangle(player.GetCapsuleData().start_pos, player.GetCapsuleData().end_pos,
				player.GetCapsuleData().r, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
			{
				// 規定距離分プレイヤーを壁の法線方向に移動させる
				// 移動後の位置を更新（移動後の場所を補正）
				fixed_pos = VAdd(fixed_pos, VScale(poly->Normal, kHitSlideLength));

				// 移動した壁ポリゴンと接触しているかどうかを判定
				for (int j = 0; j < wall_num_; j++)
				{
					// 当たっていたらループを抜ける
					poly = wall_[j];
					if (HitCheck_Capsule_Triangle(player.GetCapsuleData().start_pos, player.GetCapsuleData().end_pos,
						player.GetCapsuleData().r,  poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
					{
						isHitWall = true;
						break;
					}
				}

				// 全てのポリゴンと当たっていなかったらここでループ終了
				if (isHitWall == false)
				{
					break;
				}
			}
		}

		// 全部のポリゴンで押し出しを試みる前に
		// 全ての壁ポリゴンと接触しなくなったらループから抜ける
		if (isHitWall == false)
		{
			break;
		}


	}



	return fixed_pos;
}


VECTOR Stage::CheckHitWithFloor(Player& player, const VECTOR& check_position)
{
	VECTOR fixed_pos = check_position;

	// 床の数が無かったら早期リターン
	if (floor_num_ == 0)
	{
		return fixed_pos;
	}

	// ジャンプ中且つ上昇中の場合は処理を分岐
	


	return fixed_pos;
}


/*------------public------------*/


void Stage::Draw()
{
	MATRIX scale_matrix = MGetScale(scale_);
	//行列を生成
	MATRIX pos_matrix = MGetTranslate(position_);

	//モデルの行列をセットする
	if (TRUE)
	{
		matrix_ = MMult(scale_matrix, pos_matrix);
	}
	else
	{
		matrix_ = pos_matrix;
	}


	MV1SetMatrix(model_, matrix_);
	MV1DrawModel(model_);
}


VECTOR Stage::CheckCollision(Player& player, const VECTOR& velocity)
{
	VECTOR old_pos = player.GetPos();
	VECTOR next_pos = VAdd(old_pos, velocity);

	// HACK: ステージポリゴンが複数ある場合、ここが繰り返し処理になる
	{
		auto hit_dim = MV1CollCheck_Capsule(model_, -1, player.GetCapsuleData().start_pos,
			player.GetCapsuleData().end_pos, player.GetCapsuleData().r);

		AnalyzeWallAndFloor(hit_dim, old_pos);




	}

	return next_pos;

}





