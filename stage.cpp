#include<iostream>
#define _USE_MATH_DEFINES
#include<math.h>

#include"DxLib.h"
#include"Player.h"
#include"stage.h"


VECTOR GetProjectionVector(const VECTOR& vector, const VECTOR& vector2)
{
	VECTOR projection = VGet(0.f, 0.f, 0.f);

	//分母
	float denominator;

	denominator = (vector.x * vector.x) + (vector.y * vector.y)
		+ (vector.z * vector.z);

	//分子
	float molecule;

	molecule = vector.x * vector2.x + vector.y * vector2.y + vector.z * vector2.z;

	projection = VScale(vector, (molecule / denominator));


	return projection;
}

Stage::Stage(int model_handle, VECTOR pos, float scale)
	: BaseObject(pos, model_handle)
	, scale_(VGet(scale,scale,scale))
	, wall_num_(0)
	, floor_num_(0)
	, wall_{ nullptr }
	, floor_{ nullptr }
{
	MV1SetupCollInfo(model_, -1);

	


	MATRIX scale_matrix = MGetScale(scale_);
	//行列を生成
	MATRIX pos_matrix = MGetTranslate(position_);
	matrix_ = MMult(scale_matrix, pos_matrix);


	MV1SetMatrix(model_, matrix_);
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
	if (player.GetNowState() == PlayerState::kJump && player.GetVelocity().y > 0.0f)
	{
		// 天井に頭をぶつける処理を行う
		// 一番低い天井にぶつける為の判定用変数を初期化
		bool isHitRoof = false;
		float minY = 0.0f;

		// 床ポリゴンの数だけ繰り返し
		for (int i = 0; i < floor_num_; i++)
		{
			auto poly = floor_[i];	// i番目の床ポリゴンのアドレス

			// 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定
			HITRESULT_LINE lineResult;	// 線分とポリゴンとの当たり判定の結果を代入する構造体
			lineResult = HitCheck_Line_Triangle(player.GetCapsuleData().start_pos, 
				player.GetCapsuleData().end_pos, poly->Position[0], poly->Position[1], poly->Position[2]);

			// 接触していなかったら何もしない
			if (lineResult.HitFlag == TRUE)
			{
				// 既にポリゴンに当たっていて、且つ今まで検出した天井ポリゴンより高い場合は何もしない
				if (!(isHitRoof == true && minY < lineResult.Position.y))
				{
					// ポリゴンに当たったフラグを立てる
					isHitRoof = true;

					// 接触したＹ座標を保存する
					minY = lineResult.Position.y;
				}
			}
		}

		// 接触したポリゴンがあれば
		if (isHitRoof == true)
		{
			// 接触した場合はプレイヤーのＹ座標を接触座標を元に更新
			fixed_pos.y = minY - (player.GetCapsuleData().end_pos.y - player.GetCapsuleData().start_pos.y);
			player.OnHitRoof();
		}

	}
	else
	{
		bool isHitFloor = false;
		float maxY = 0.0f;

		//床ポリゴンの数だけ繰り返し
		for (int i = 0; i < floor_num_; i++)
		{
			auto poly = floor_[i];
			
			// ジャンプ中かどうかで処理を分岐
			HITRESULT_LINE lineResult;	// 線分とポリゴンとの当たり判定の結果を代入する構造体
			if (player.GetNowState() == PlayerState::kJump)
			{
				// ジャンプ中の場合は頭の先から足先より少し低い位置の間で当たっているかを判定
				lineResult = HitCheck_Line_Triangle(player.GetCapsuleData().end_pos,
					VGet(player.GetCapsuleData().start_pos.x, player.GetCapsuleData().start_pos.y - 1.0f, player.GetCapsuleData().start_pos.z)
					,poly->Position[0], poly->Position[1], poly->Position[2]);
			}
			else
			{
				// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
				lineResult = HitCheck_Line_Triangle(player.GetCapsuleData().end_pos,
					VGet(player.GetCapsuleData().start_pos.x, player.GetCapsuleData().start_pos.y - 40.0f, player.GetCapsuleData().start_pos.z)
					, poly->Position[0], poly->Position[1], poly->Position[2]);
			}

			// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
			if (lineResult.HitFlag == TRUE)
			{
				if (!(isHitFloor == true && maxY > lineResult.Position.y))
				{
					// 接触したＹ座標を保存する
					isHitFloor = true;
					maxY = lineResult.Position.y;
				}
			}
		}

		// 床ポリゴンに当たった
		if (isHitFloor == true)
		{
			// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
			fixed_pos.y = maxY;

			// 床に当たった時
			player.OnHitFloor();
		}
		else
		{
			// 床コリジョンに当たっていなくて且つジャンプ状態ではなかった場合は落下状態
			//player.OnFall();
		}

	}

	return fixed_pos;
}


VECTOR Stage::CheckEntityCollisionFixedPOs(Player& player, MV1_COLL_RESULT_POLY* entity, int hit_num, const VECTOR& pos, const VECTOR& vel)
{
	VECTOR old_pos = pos;
	VECTOR offset_vel =vel;
	VECTOR next_pos = VAdd(old_pos, offset_vel);

	//壁に当たっているのを検知する
	
	//printfDx("x:%.2f,y:%.2f,z:%.2f\n", old_pos.x, old_pos.y, old_pos.z);
	//printfDx("y:%.2f\n",old_pos.y);

	auto old_player_capsule = player.GetCapsuleData();
	auto next_player_capsule = old_player_capsule;

	//未来のカプセルの座標を更新
	next_player_capsule.start_pos = next_pos;
	next_player_capsule.start_pos.y += next_player_capsule.r;
	next_player_capsule.end_pos = next_player_capsule.start_pos;
	next_player_capsule.end_pos.y += next_player_capsule.vertical_num;


	
	for (int k = 0; k < kHitTryNum; k++)
	{
		bool is_hit = FALSE;
		for (int i = 0; i < hit_num; i++)
		{
			auto poly = entity[i];



			//衝突しているとき
			if (HitCheck_Capsule_Triangle(next_player_capsule.start_pos, next_player_capsule.end_pos
				, next_player_capsule.r, poly.Position[0], poly.Position[1], poly.Position[2]))
			{

				//中点を出す
				VECTOR poly_center_pos =
					VGet((poly.Position[0].x + poly.Position[1].x + poly.Position[2].x) / 3,
						(poly.Position[0].y + poly.Position[1].y + poly.Position[2].y) / 3,
						(poly.Position[0].z + poly.Position[1].z + poly.Position[2].z) / 3
					);


				/*----------ここからはセグメントのやつ(capsuleのstart_posのやつ)------------*/

				//ポリゴンの中点からの距離を見てから、そのあと正射影ベクトルを出す。
				//センターからの距離
				VECTOR poly_to_old = VSub(old_pos, poly_center_pos);			//old
				VECTOR poly_to_next;

				if (FALSE)
				{
					poly_to_next = VSub(next_player_capsule.start_pos,
						poly_center_pos);			//next
				}
				else
				{
					poly_to_next = VSub(next_pos,
						poly_center_pos);			//next
				}





				//正射影ベクトルを出す
				VECTOR poly_to_old_proj_vec = GetProjectionVector(poly.Normal, poly_to_old);
				VECTOR poly_to_next_proj_vec = GetProjectionVector(poly.Normal, poly_to_next);

				if (FALSE)
				{

				}
				else
				{

					// 新規の処理
					// この中で壁ずり

					if (TRUE)
					{
						// 地面の判定(法線のY座標が0以下なら)
						if (poly.Normal.y <= 0.f)
						{


						}
						else
						{

							player.SetIsGround(TRUE);
							player.ResetFallSpeed();
						}

						//ポリゴンの中点からの距離を見てから、そのあと正射影ベクトルを出す。
							//センターからの距離
						poly_to_old = VSub(old_pos, poly_center_pos);			//old
						poly_to_next = VSub(next_pos, poly_center_pos);			//next

						//nowのpoly.normalの向きを逆にする
						auto reverce_norm = VScale(poly.Normal, -1);

						//正射影ベクトルを出す
						poly_to_old_proj_vec = GetProjectionVector(reverce_norm, poly_to_old);

						//
						poly_to_next_proj_vec = GetProjectionVector(poly.Normal, poly_to_next);

						// 各ベクターの大きさを出す
						float poly_to_old_size = fabs((sqrt((poly_to_old_proj_vec.x * poly_to_old_proj_vec.x) +
							(poly_to_old_proj_vec.y * poly_to_old_proj_vec.y) + (poly_to_old_proj_vec.z * poly_to_old_proj_vec.z))));

						float poly_to_next_size = fabs((sqrt((poly_to_next_proj_vec.x * poly_to_next_proj_vec.x) +
							(poly_to_next_proj_vec.y * poly_to_next_proj_vec.y) + (poly_to_next_proj_vec.z * poly_to_next_proj_vec.z))));

						// その比をみて、全体の移動量にかける。調べたい比 単体/全体
						float ratio = fabs(poly_to_old_size) / (fabs(poly_to_old_size) + poly_to_next_size);

						//ここ次の距離までの移動量のnextの比
						float next_ratio = poly_to_next_size / (fabs(poly_to_old_size) + poly_to_next_size);


						if (TRUE)
						{
							VECTOR offset_pos = VAdd(next_pos, VScale(poly.Normal, poly_to_next_size));

							if (poly.Normal.y <= 0.f)
							{
								//半径分押し出す
								//offset_pos = VAdd(offset_pos, VScale(reverce_norm, player.GetCapsuleData().r));

							}

							offset_vel = VSub(offset_pos, old_pos);
						}
						else
						{
							// vectorにかける
							offset_vel = VScale(offset_vel, ratio);
						}



						//offset分足したカプセルの座標
						next_player_capsule.start_pos = VAdd(old_pos, offset_vel);
						next_player_capsule.start_pos.y += next_player_capsule.r;
						next_player_capsule.end_pos = next_player_capsule.start_pos;
						next_player_capsule.end_pos.y += next_player_capsule.vertical_num;

						//移動後にもう一度何かと当たっているのかを調べる
						for (int j = 0; j < hit_num; j++)
						{
							poly = entity[j];

							if (HitCheck_Capsule_Triangle(next_player_capsule.start_pos, next_player_capsule.end_pos
								, next_player_capsule.r, poly.Position[0], poly.Position[1], poly.Position[2]))
							{
								is_hit = TRUE;
								break;
							}

						}


						//全てのポリゴンと当たっていない場合ループ終了
						if (!is_hit)
						{
							break;
						}
					}


				}



			}




		}


		// 全部のポリゴンで押し出しを試みる前に
		// 全ての壁ポリゴンと接触しなくなったらループから抜ける
		if (is_hit == false)
		{
			break;
		}

	}

	return VAdd(old_pos,offset_vel);
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
	VECTOR offset_vel = velocity;
	VECTOR next_pos = VAdd(old_pos, offset_vel);
	
	//壁に当たっているのを検知する
	bool is_hit_wall = FALSE;

	//printfDx("x:%.2f,y:%.2f,z:%.2f\n", old_pos.x, old_pos.y, old_pos.z);
	//printfDx("y:%.2f\n",old_pos.y);

	auto old_player_capsule = player.GetCapsuleData();
	auto next_player_capsule = old_player_capsule;

	//未来のカプセルの座標を更新
	next_player_capsule.start_pos = next_pos;
	next_player_capsule.start_pos.y += next_player_capsule.r;
	next_player_capsule.end_pos = next_player_capsule.start_pos;
	next_player_capsule.end_pos.y += next_player_capsule.vertical_num;
	//printfDx("next_cap_r:%.2f\n", next_player_capsule.r);
	// ミライのベクトルから現在のベクトルまでのカプセルを作
		// 各カプセルの中心の座標を検出する
		// それをCapsuleDataのstart_posとend_posに当てはめる
		// 半径は(vertical_num + (r * 0.5f))

	//検出するカプセルの更新
	next_to_old_cap_.start_pos = VScale(VAdd(next_player_capsule.start_pos, next_player_capsule.end_pos), 0.5f);
	next_to_old_cap_.end_pos = VScale(VAdd(old_player_capsule.start_pos, old_player_capsule.end_pos), 0.5f);

	next_to_old_cap_.r = (old_player_capsule.r + (next_to_old_cap_.vertical_num * 0.5f));
	next_to_old_cap_.vertical_num = (VSize(VSub(next_player_capsule.end_pos, next_player_capsule.start_pos)));

	// 当たり判定を素晴らしくしましょう
	// 壁と地面で分けたほうがよさそうです


	// HACK: ステージポリゴンが複数ある場合、ここが繰り返し処理になる
	{
		// プレイヤーの周囲にあるステージポリゴンを取得する
		// ( 検出する範囲は移動距離も考慮する )
		auto hit_dim = MV1CollCheck_Capsule(model_, 
			-1, next_to_old_cap_.start_pos, next_to_old_cap_.end_pos,
			(next_to_old_cap_.r));

		if (before_hit_num_ != hit_dim.HitNum)
		{
			//printfDx("%d\n", hit_dim.HitNum);
			before_hit_num_ = hit_dim.HitNum;
		}

		
		

		//printfDx("r:%.2f\n", next_player_capsule.r);

		// 何も触れていないときに重力判定をする
		if (hit_dim.HitNum == 0)
		{

			if (player.GetFallSpeed() == 0.f)
			{

				auto check_capsule = next_player_capsule;
				//少し下を見る
				check_capsule.start_pos = VAdd(old_pos, VGet(0.f, -0.1f, 0.f));
				check_capsule.start_pos.y += check_capsule.r;
				check_capsule.end_pos = check_capsule.start_pos;
				check_capsule.end_pos.y += check_capsule.vertical_num;

				auto gravity_check_hit_dim = MV1CollCheck_Capsule(model_,
					-1, check_capsule.start_pos, check_capsule.end_pos,
					check_capsule.r);

				//下の座標を見た時何にも触れていなかったら重力あり
				if (gravity_check_hit_dim.HitNum == 0)
				{
					player.SetIsGround(FALSE);
				}
				else
				{
					for (int i = 0; i < gravity_check_hit_dim.HitNum; i++)
					{
						auto poly = gravity_check_hit_dim.Dim[i];
						if (HitCheck_Capsule_Triangle(check_capsule.start_pos, check_capsule.end_pos
							, check_capsule.r, poly.Position[0], poly.Position[1], poly.Position[2]))
						{
							player.SetIsGround(TRUE);
						}
						else
						{
							player.SetIsGround(FALSE);
						}
					}
				}

				//ポリゴン情報を解放する
				MV1CollResultPolyDimTerminate(gravity_check_hit_dim);

			}



		}

		/*
		//さきにかべに当たっているのなら検知させておく
		for (int i = 0; i < hit_dim.HitNum; i++)
		{
			auto poly = hit_dim.Dim[i];

			if (poly.Normal.y <= 0.f)
			{
				//壁
				is_hit_wall = TRUE;
				wall_[wall_num_] = &poly;
				wall_num_++;
			}
			else
			{
				//床
				floor_[floor_num_] = &poly;
				floor_num_++;
			}
		}
		*/
		
		

		for (int k = 0; k < kHitTryNum; k++)
		{
			bool is_hit = FALSE;
			for (int i = 0; i < hit_dim.HitNum; i++)
			{
				auto poly = hit_dim.Dim[i];



				//衝突しているとき
				if (HitCheck_Capsule_Triangle(next_player_capsule.start_pos, next_player_capsule.end_pos
					, next_player_capsule.r, poly.Position[0], poly.Position[1], poly.Position[2]))
				{

					//中点を出す
					VECTOR poly_center_pos =
						VGet((poly.Position[0].x + poly.Position[1].x + poly.Position[2].x) / 3,
							(poly.Position[0].y + poly.Position[1].y + poly.Position[2].y) / 3,
							(poly.Position[0].z + poly.Position[1].z + poly.Position[2].z) / 3
						);


					/*----------ここからはセグメントのやつ(capsuleのstart_posのやつ)------------*/

					//ポリゴンの中点からの距離を見てから、そのあと正射影ベクトルを出す。
					//センターからの距離
					VECTOR poly_to_old = VSub(old_pos, poly_center_pos);			//old
					VECTOR poly_to_next;

					if (FALSE)
					{
						poly_to_next = VSub(next_player_capsule.start_pos,
							poly_center_pos);			//next
					}
					else
					{
						poly_to_next = VSub(next_pos,
							poly_center_pos);			//next
					}





					//正射影ベクトルを出す
					VECTOR poly_to_old_proj_vec = GetProjectionVector(poly.Normal, poly_to_old);
					VECTOR poly_to_next_proj_vec = GetProjectionVector(poly.Normal, poly_to_next);

					if (FALSE)
					{

					}
					else
					{

						// 新規の処理
						// この中で壁ずり

						if (TRUE)
						{
							// 地面の判定(法線のY座標が0以下なら)
							if (poly.Normal.y <= 0.f)
							{


							}
							else
							{

								player.SetIsGround(TRUE);
								player.ResetFallSpeed();
							}

							//ポリゴンの中点からの距離を見てから、そのあと正射影ベクトルを出す。
								//センターからの距離
							poly_to_old = VSub(old_pos, poly_center_pos);			//old
							poly_to_next = VSub(next_pos, poly_center_pos);			//next

							//nowのpoly.normalの向きを逆にする
							auto reverce_norm = VScale(poly.Normal, -1);

							//正射影ベクトルを出す
							poly_to_old_proj_vec = GetProjectionVector(reverce_norm, poly_to_old);

							//
							poly_to_next_proj_vec = GetProjectionVector(poly.Normal, poly_to_next);

							// 各ベクターの大きさを出す
							float poly_to_old_size = fabs((sqrt((poly_to_old_proj_vec.x * poly_to_old_proj_vec.x) +
								(poly_to_old_proj_vec.y * poly_to_old_proj_vec.y) + (poly_to_old_proj_vec.z * poly_to_old_proj_vec.z))));

							float poly_to_next_size = fabs((sqrt((poly_to_next_proj_vec.x * poly_to_next_proj_vec.x) +
								(poly_to_next_proj_vec.y * poly_to_next_proj_vec.y) + (poly_to_next_proj_vec.z * poly_to_next_proj_vec.z))));

							// その比をみて、全体の移動量にかける。調べたい比 単体/全体
							float ratio = fabs(poly_to_old_size) / (fabs(poly_to_old_size) + poly_to_next_size);

							//ここ次の距離までの移動量のnextの比
							float next_ratio = poly_to_next_size / (fabs(poly_to_old_size) + poly_to_next_size);


							if (TRUE)
							{
								VECTOR offset_pos = VAdd(next_pos, VScale(poly.Normal, poly_to_next_size));

								if (poly.Normal.y <= 0.f)
								{
									//半径分押し出す
									//offset_pos = VAdd(offset_pos, VScale(reverce_norm, player.GetCapsuleData().r));

								}

								offset_vel = VSub(offset_pos, old_pos);
							}
							else
							{
								// vectorにかける
								offset_vel = VScale(offset_vel, ratio);
							}






							if (!is_hit_wall)
							{
								offset_vel = VAdd(offset_vel, VGet(0.f, 0.1f, 0.f));
							}



							//offset分足したカプセルの座標
							next_player_capsule.start_pos = VAdd(old_pos, offset_vel);
							next_player_capsule.start_pos.y += next_player_capsule.r;
							next_player_capsule.end_pos = next_player_capsule.start_pos;
							next_player_capsule.end_pos.y += next_player_capsule.vertical_num;

							//printfDx("x:%f,y:%f,z:%f\n", offset_vel.x, offset_vel.y, offset_vel.z);



							// 昔の処理(ゴミ)
							// 法線の方向に押し戻す
							//offset_vel = VSub(VSub(next_pos, old_pos), VScale(poly.Normal, kHitSlideLength));

							//offset_vel = VGet(0, 0, 0);


							//next_pos = VAdd(next_pos, VScale(poly.Normal, 0.1f));
							//printfDx("x:%f,y:%f,z:%f\n", poly.Normal.x, poly.Normal.y, poly.Normal.z);
							//offset_vel = VSub(VSub(next_pos, old_pos),offset_vel);



							//移動後にもう一度何かと当たっているのかを調べる
							for (int j = 0; j < hit_dim.HitNum; j++)
							{
								poly = hit_dim.Dim[j];

								if (HitCheck_Capsule_Triangle(next_player_capsule.start_pos, next_player_capsule.end_pos
									, next_player_capsule.r, poly.Position[0], poly.Position[1], poly.Position[2]))
								{
									is_hit = TRUE;
									break;
								}

							}


							//全てのポリゴンと当たっていない場合ループ終了
							if (!is_hit)
							{
								break;
							}
						}


					}



				}




			}


			// 全部のポリゴンで押し出しを試みる前に
			// 全ての壁ポリゴンと接触しなくなったらループから抜ける
			if (is_hit == false)
			{
				break;
			}

		}
		

		

		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hit_dim);
	}

	next_pos = VAdd(old_pos, offset_vel);

	//printfDx("x:%.2f,y:%.2f,z:%.2f\n", offset_vel.x, offset_vel.y, offset_vel.z);

	return next_pos;

}







