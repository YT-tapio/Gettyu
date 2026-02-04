#include<iostream>
#define _USE_MATH_DEFINES
#include<math.h>

#include"DxLib.h"
#include"player.h"
#include"stage.h"
#include"vector_assistant.h"
#include"collision_base.h"
#include"collision_sphere.h"
#include"collision_capsule.h"
#include"const_rad.h"

Stage::Stage(const char* path, VECTOR pos, float scale)
	: ObjectBase(pos, VectorAssistant::GetZeroVec(), VectorAssistant::GetSame3DVec(scale), path)
	, wall_num_(0)
	, floor_num_(0)
	, wall_{ nullptr }
	, floor_{ nullptr }
{
	MV1SetupCollInfo(model_, -1);

	MATRIX scale_matrix = MGetScale(scale_);
	//行列を生成
	MATRIX pos_matrix = MGetTranslate(pos_);
	mat_ = MMult(scale_matrix, pos_matrix);

	next_to_old_cap_ = std::make_shared<CollisionCapsule>(VectorAssistant::GetZeroVec(), VectorAssistant::GetZeroVec(), 0.f);

	MV1SetMatrix(model_, mat_);
	
}


Stage::~Stage()
{
	
}


VECTOR Stage::CheckEntityCollisionOffsetVelocity(MV1_COLL_RESULT_POLY* entity, int hit_num, std::shared_ptr<ColliderBase> obj_coll, const VECTOR& velocity)
{
	VECTOR offset_vel	= velocity;
	auto old_coll		= obj_coll->Clone();
	auto next_coll		= obj_coll->Clone();

	next_coll->Update(offset_vel);
	
	VECTOR old_pos		= old_coll->GetPos();
	VECTOR next_pos		= next_coll->GetPos();

	VECTOR capsule_start_pos	= old_coll->GetCenterPos();
	VECTOR capsule_end_pos		= next_coll->GetCenterPos();
	float coll_radius			= old_coll->GetWidth();

	for (int k = 0; k < kHitTryNum; k++)
	{
		bool is_hit = FALSE;
		for (int i = 0; i < hit_num; i++)
		{
			//ポリゴンを代入
			auto poly = entity[i];
			//衝突しているとき
			if (next_coll->IsHitTriangle(poly.Position[0], poly.Position[1], poly.Position[2]) || old_coll->IsHitTriangle(poly.Position[0], poly.Position[1], poly.Position[2]) ||
				(HitCheck_Line_Triangle(old_coll->GetPos(), next_coll->GetPos(), poly.Position[0], poly.Position[1], poly.Position[2]).HitFlag) == 1)
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
				//カプセルの開始の位置
				VECTOR poly_to_old				= VectorAssistant::GetZeroVec();			//old
				VECTOR poly_to_next				= VectorAssistant::GetZeroVec();

				//正射影ベクトルを出す
				VECTOR poly_to_old_proj_vec		= VectorAssistant::GetZeroVec();
				VECTOR poly_to_next_proj_vec	= VectorAssistant::GetZeroVec();

				//ポリゴンの中点からの距離を見てから、そのあと正射影ベクトルを出す。
				//センターからの距離
				poly_to_old = VSub(old_pos, poly_center_pos);			//old
				poly_to_next = VSub(next_pos, poly_center_pos);			//next

				//nowのpoly.normalの向きを逆にする
				auto reverce_norm = VScale(poly.Normal, -1);

				// 正射影ベクトルを出す(元のposから)
				poly_to_old_proj_vec = VectorAssistant::GetProj(poly.Normal, poly_to_old);

				// 次のposから
				poly_to_next_proj_vec = VectorAssistant::GetProj(reverce_norm, poly_to_next);


				// velocityを足し終わった後に法線分三角形にめり込んでいる分を押し出す
				VECTOR offset_pos	= VSub(next_pos, poly_to_next_proj_vec);
				offset_pos			= VAdd(offset_pos, VScale(poly.Normal, old_coll->GetRadius()));

				//　元のposから、offsetした後のposの差を見る
				offset_vel = VSub(offset_pos, old_pos);

				//offset分足したカプセルの座標
				next_coll = old_coll->Clone();
				next_coll->Update(offset_vel);
				next_pos = next_coll->GetPos();
				capsule_end_pos = next_coll->GetCenterPos();

				//当たり判定検出の位置を更新
				next_to_old_cap_ = std::make_shared<CollisionCapsule>(capsule_start_pos, capsule_end_pos, coll_radius);

				//移動後にもう一度何かと当たっているのかを調べる
				for (int j = 0; j < hit_num; j++)
				{
					poly = entity[j];

					if (old_coll->IsHitTriangle(poly.Position[0], poly.Position[1], poly.Position[2]) || next_coll->IsHitTriangle(poly.Position[0], poly.Position[1], poly.Position[2])
						|| (HitCheck_Line_Triangle(old_pos, next_pos, poly.Position[0], poly.Position[1], poly.Position[2]).HitFlag) == 1)
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

	return offset_vel;
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

void Stage::MakeCollCheckCapsule(CapsuleData old_cap, CapsuleData next_cap)
{

}

VECTOR Stage::GetSegmentPolySuckVel(const VECTOR& start_pos, const VECTOR& end_pos, const VECTOR& poly_center_pos, const VECTOR& poly_norm)
{

	VECTOR center_to_start	= VSub(start_pos, poly_center_pos);
	VECTOR center_to_end	= VSub(end_pos, poly_center_pos);

	
	VECTOR center_to_end_proj_vec	= VectorAssistant::GetZeroVec();

	//nowのpoly.normalの向きを逆にする
	auto reverce_norm = VScale(poly_norm, -1);

	center_to_end_proj_vec		= VectorAssistant::GetProj(reverce_norm, center_to_end);

	VECTOR offset_pos = VSub(end_pos, center_to_end_proj_vec);

	return VSub(offset_pos,start_pos);
}

VECTOR Stage::GetSegmentPolyHitPos(const VECTOR& start_pos, const VECTOR& end_pos, const VECTOR& poly_center_pos, const VECTOR& poly_norm)
{

	VECTOR hit_pos			= VectorAssistant::GetZeroVec();			// 
	VECTOR vel				= VSub(end_pos, start_pos);					// 移動量
	VECTOR center_to_start	= VSub(start_pos, poly_center_pos);			// ポリゴンの中心座標から始点までの距離
	VECTOR center_to_end	= VSub(end_pos, poly_center_pos);			// ポリゴンの中心座標から終点までの距離
	VECTOR center_to_start_proj_vec = VectorAssistant::GetZeroVec();	// 
	VECTOR center_to_end_proj_vec	= VectorAssistant::GetZeroVec();	// 

	//nowのpoly.normalの向きを逆にする
	auto reverce_norm = VScale(poly_norm, -1);

	center_to_start_proj_vec	= VectorAssistant::GetProj(poly_norm, center_to_start);
	center_to_end_proj_vec		= VectorAssistant::GetProj(reverce_norm, center_to_end);

	// 正射影ベクトルの比を見る

	float ratio = VSize(center_to_start_proj_vec) / (VSize(center_to_start_proj_vec) + VSize(center_to_end_proj_vec));
	
	

	return VAdd(start_pos, VScale(vel, ratio));
}

bool Stage::IsStair(const VECTOR& poly_pos, const VECTOR& entity_pos,const float& r)
{
	// polyの高さがentityのposよりも小さく半径内なら
	/*
	if (entity_pos.y > poly_pos.y)
	{
		//polyとentityの距離が半径分離れている
		float sub = entity_pos.y - poly_pos.y;
		return sub < r;		//半径よりも低い
	}
	*/
	
	
	//VECTOR dist = VSub(poly_pos, entity_pos);

	return poly_pos.y < entity_pos.y;
}

bool Stage::IsFlat(const VECTOR& norm)
{
	return (norm.y != 0.f );
}

bool Stage::CheckTriangleAreaSize(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3,const  VECTOR& object_pos)
{
	// 普通に頂点が一つでも低かったら判定させるようにする

	if (object_pos.y > pos1.y)
	{
		return FALSE;
	}

	if (object_pos.y > pos2.y)
	{
		return FALSE;
	}

	if (object_pos.y > pos3.y)
	{
		return FALSE;
	}


	// 面積がでかいものは通す
	return TRUE;
}

bool Stage::CheckDownColl(const std::shared_ptr<ColliderBase> coll)
{
	bool flag = FALSE;				//こいつが返す
	const VECTOR kDownVel = VGet(0.f, -0.2f, 0.f);
	//最初になにも当たっていないかを確認
	
	auto hit_dim = coll->GetCollInfo(model_);

	if (hit_dim.HitNum == 0)
	{
		//下に下げるためのcoll
		auto down_coll = coll->Clone();

		down_coll->Update(kDownVel);

		hit_dim = down_coll->GetCollInfo(model_);

		//下に少し下げてもなんとも当たらないのなら
		if (hit_dim.HitNum == 0)
		{
			flag = FALSE;
		}
		else
		{
			flag = TRUE;
		}
	}
	else
	{
		// 何かしらにあたっているときに地面じゃないなら
		flag = TRUE;
	}

	//データ開放
	MV1CollResultPolyDimTerminate(hit_dim);

	return flag;
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
		bool	isHitFloor	= false;
		float	maxY		= 0.0f;

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

VECTOR Stage::CheckFootProjectionPos(const VECTOR& old_pos,const VECTOR& next_pos, const float& r)
{
	const float kSizeSegment = 1.f;

	const float kOffsetDist = r + 2.f;	// レイの許容範囲
	VECTOR projection_pos = next_pos;

	VECTOR old_segment_start_pos	= old_pos;				// カプセルの始点
	VECTOR old_segment_end_pos		= VAdd(old_segment_start_pos, VGet(0.f, -(kOffsetDist), 0.f));				// カプセルの始点からセグメントを伸ばす
	auto old_segment_hit_dim = MV1CollCheck_LineDim(model_, -1, old_segment_start_pos, old_segment_end_pos);

	bool flag = (old_segment_hit_dim.HitNum > 0);
	//
	flag = TRUE;
	if (flag)
	{
		const float next_segment_size = r + kSizeSegment;
		//printfDx("地面です : ");
		// sphereを少し下げたときに何もないときに投映を開始
		//auto next_sphere_hit_dim = MV1CollCheck_Sphere(model_, -1, VAdd(next_pos, VGet(0.f, -0.1f, 0.f)), r);
		
		//投映する

		VECTOR next_segment_start_pos = next_pos;			// カプセルの始点
		VECTOR next_segment_end_pos = VAdd(next_segment_start_pos, VGet(0.f, -(next_segment_size), 0.f));				// カプセルの始点からセグメントを伸ばす
		auto next_segment_hit_dim = MV1CollCheck_LineDim(model_, -1, next_segment_start_pos, next_segment_end_pos);		// 
		
		for (int i = 0; i < next_segment_hit_dim.HitNum; i++)
		{
			auto poly = next_segment_hit_dim.Dim[i];

			// ポリゴンが対象のy座標よりも高い場合を除きたい
			VECTOR entity_pos = VAdd(next_pos, VGet(0.f, -r, 0.f));
			if (!((poly.Position[0].y < entity_pos.y) && (poly.Position[1].y < entity_pos.y) && (poly.Position[2].y < entity_pos.y))) { continue; }
			
			auto hit_check = HitCheck_Line_Triangle(next_segment_start_pos, next_segment_end_pos, poly.Position[0], poly.Position[1], poly.Position[2]);

			

			if (hit_check.HitFlag)
			{
				//中点を出す
				VECTOR poly_center_pos =
					VGet((poly.Position[0].x + poly.Position[1].x + poly.Position[2].x) / 3,
						(poly.Position[0].y + poly.Position[1].y + poly.Position[2].y) / 3,
						(poly.Position[0].z + poly.Position[1].z + poly.Position[2].z) / 3
					);

				printfDx("投影\n");
				next_segment_end_pos = hit_check.Position;
				projection_pos = VAdd(next_segment_end_pos, VScale(poly.Normal, r));
				if (FALSE)
				{
					VECTOR poly_to_end_pos = VSub(next_segment_end_pos, poly_center_pos);

					VECTOR poly_near_dist = VectorAssistant::GetProj(poly.Normal, poly_to_end_pos);	// 最短距離
					VECTOR radius_dist = VScale(VNorm(poly_near_dist), r);

					VECTOR penetration_dist = VSub(radius_dist, poly_near_dist);				// めり込み量
					projection_pos = VAdd(projection_pos, penetration_dist);			// 

				}

				if (FALSE)
				{
					if (hit_check.HitFlag)
					{
						VECTOR hit_pos = GetSegmentPolyHitPos(next_segment_start_pos, next_segment_end_pos, poly_center_pos, poly.Normal);
						next_segment_end_pos = hit_pos;
						projection_pos = VAdd(next_segment_end_pos, VScale(VGet(0.f, 1.f, 0.f), r));
					}
				}
			}
		}
		
		MV1CollResultPolyDimTerminate(next_segment_hit_dim);
		
		/*
		if (next_sphere_hit_dim.HitNum == 0)
		{
			
		}
		MV1CollResultPolyDimTerminate(next_sphere_hit_dim);
		*/
		
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(old_segment_hit_dim);
	

	return projection_pos;
}

/*------------public------------*/

void Stage::Init()
{

}

void Stage::Update()
{

}

void Stage::Draw()
{
	MATRIX scale_matrix = MGetScale(scale_);
	//行列を生成
	MATRIX pos_matrix = MGetTranslate(pos_);

	//モデルの行列をセットする
	if (TRUE)
	{
		mat_ = MMult(scale_matrix, pos_matrix);
	}
	else
	{
		mat_ = pos_matrix;
	}

	MV1SetMatrix(model_, mat_);
	MV1DrawModel(model_);
	if (FALSE)
	{
		// 優先される床(青)
		for (auto& poly : prioritize_floor_polys_)
		{
			DrawTriangle3D(poly.pos[0], poly.pos[1], poly.pos[2], GetColor(0, 0, 255), FALSE);
		}

		// 壁(緑)
		for (auto& poly : wall_polys_)
		{
			DrawTriangle3D(poly.pos[0], poly.pos[1], poly.pos[2], GetColor(0, 255, 0), FALSE);
		}

		// 床(赤)
		for (auto& poly : floor_polys_)
		{
			DrawTriangle3D(poly.pos[0], poly.pos[1], poly.pos[2], GetColor(255, 0, 0), FALSE);
		}

		for (auto& data : prioritize_floor_poly_data_)
		{
			DrawLine3D(data.center_pos, VAdd(data.center_pos, VScale(data.norm, 10.f)), GetColor(255, 0, 0));			// そのまま (赤)
			DrawLine3D(data.center_pos, VAdd(data.center_pos, VScale(data.x_angle_norm, 10.f)), GetColor(0, 255, 0));	// x軸回転  (緑)
			DrawLine3D(data.center_pos, VAdd(data.center_pos, VScale(data.z_angle_norm, 10.f)), GetColor(0, 0, 255));	// y軸回転  (青)


			//DrawLine3D(data.center_pos, VAdd(data.center_pos, VScale(data.z_angle_norm, 10.f)), GetColor(0, 0, 2))
			//DrawLine3D(data.center_pos, VAdd(data.center_pos, VScale(VectorAssistant::VGetRotRadZ(data.norm, 90), 10.f)), GetColor(0, 0, 0));

		}

		DrawSphere3D(rem_hit_pos, 0.5f, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);

	}
	
}

void Stage::Debug()
{
	next_to_old_cap_->Debug();
}

VECTOR Stage::CheckCollision(std::shared_ptr<ColliderBase> object_coll, const VECTOR& velocity)
{
	VECTOR offset_vel = velocity;
	
	//壁に当たっているのを検知する
	bool is_hit_wall = FALSE;

	// wall_num_,floor_num_の初期化
	wall_num_	= 0;
	floor_num_	= 0;
	prioritize_floor_num_ = 0;
	// 今の当たり判定は未来のカプセルのとこだけになっているので、カプセルを大ききくしたやつにする(nowとnextの合計のもの)

	// 新しくこいつで当たり判定を行う
	auto old_coll	= object_coll->Clone();
	auto next_coll	= object_coll->Clone();

	prioritize_floor_poly_data_.clear();

	next_coll->Update(offset_vel);

	
	VECTOR old_pos			= old_coll->GetPos();
	VECTOR next_pos			= next_coll->GetPos();

	VECTOR capsule_start_pos		= old_coll->GetCenterPos();
	VECTOR capsule_end_pos			= next_coll->GetCenterPos();
	float coll_radius				= old_coll->GetWidth();
	//当たり判定の検出のカプセルを作る
	next_to_old_cap_ = std::make_shared<CollisionCapsule>(capsule_start_pos, capsule_end_pos, coll_radius);
	prioritize_floor_polys_.clear();
	floor_polys_.clear();
	wall_polys_.clear();
	// HACK: ステージポリゴンが複数ある場合、ここが繰り返し処理になる
	{
		if (FALSE)
		{
			if (VSize(velocity) != 0.f && (velocity.y <= 0.f && velocity.y >= -0.1f))
			{
				auto foot_projection_pos = CheckFootProjectionPos(old_pos, next_pos, next_coll->GetRadius());
				//offset_vel = VScale(VNorm(VSub(foot_projection_pos, old_pos)), VSize(velocity));
				offset_vel = VScale(VNorm(VSub(foot_projection_pos, old_pos)), VSize(velocity));			// velocityを調整できるように
				rem_hit_pos = VAdd(VAdd(old_pos, offset_vel), VGet(0.f, -old_coll->GetRadius(), 0.f));
			}
		}
		
		
		

		// プレイヤーの周囲にあるステージポリゴンを取得する
		// ( 検出する範囲は移動距離も考慮する )
		auto hit_dim = next_to_old_cap_->GetCollInfo(model_);

		const float kWallRad = kOneRad * 80.f;
		for (int i = 0; i < hit_dim.HitNum; i++)
		{
			auto norm_vertical_dot = VDot(VGet(0.f, 1.f, 0.f), VNorm(hit_dim.Dim[i].Normal));	//真上に線を伸ばした時の角度
			float rad = acosf(norm_vertical_dot);
			PolyVertexPos vertex;

			for (int j = 0; j < kVertex; j++)
			{
				vertex.pos[j] = hit_dim.Dim[i].Position[j];
			}

			if (rad > kWallRad)
			{
				// 壁
				wall_[wall_num_] = &hit_dim.Dim[i];
				wall_num_++;

				wall_polys_.push_back(vertex);
			}
			else
			{
				auto poly = hit_dim.Dim[i];
				//中点を出す
				VECTOR poly_center_pos =
					VGet((poly.Position[0].x + poly.Position[1].x + poly.Position[2].x) / 3,
						(poly.Position[0].y + poly.Position[1].y + poly.Position[2].y) / 3,
						(poly.Position[0].z + poly.Position[1].z + poly.Position[2].z) / 3
					);

				if (next_pos.y < poly_center_pos.y)
				{
					// 床
					floor_[floor_num_] = &hit_dim.Dim[i];
					floor_num_++;
					floor_polys_.push_back(vertex);
				}
				else
				{
					// 床
					prioritize_floor_[prioritize_floor_num_] = &hit_dim.Dim[i];
					prioritize_floor_num_++;
					prioritize_floor_polys_.push_back(vertex);

					PrioritizeFloorPolyData data;
					data.center_pos = poly_center_pos;
					data.norm = poly.Normal;
					data.x_angle_norm = VectorAssistant::VGetRotRadX(poly.Normal,-90);
					// data.right_angle_norm = VectorAssistant::VGetRotRadY(data.right_angle_norm, -90);
					// data.right_angle_norm = VectorAssistant::VGetRotRadZ(data.right_angle_norm, -90);
					data.z_angle_norm = VectorAssistant::VGetRotRadZ(poly.Normal, -90);
					//data.left_angle_norm = VectorAssistant::VGetRotRadY(data.left_angle_norm, 90);
					//data.left_angle_norm = VectorAssistant::VGetRotRadZ(data.left_angle_norm, 90);
					
					prioritize_floor_poly_data_.push_back(data);

				}
				
				
			}

			

			//all_poly_[i] = &hit_dim.Dim[i];
		}

		int all_poly_num = 0;

		for (int i = 0; i < prioritize_floor_num_; i++)
		{
			all_poly_[all_poly_num] = prioritize_floor_[i];
			all_poly_num++;
		}

		for (int i = 0; i < wall_num_; i++)
		{
			all_poly_[all_poly_num] = wall_[i];
			all_poly_num++;
		}
		
		for (int i = 0; i < floor_num_; i++)
		{
			auto poly = floor_[i];
			//

			if (CheckTriangleAreaSize(poly->Position[0], poly->Position[1], poly->Position[2],old_pos)) { continue; }

			all_poly_[all_poly_num] = floor_[i];
			all_poly_num++;
		}

		
		bool is_projection = TRUE;

		for (int k = 0; k < kHitTryNum; k++)
		{
			bool is_hit_prioritize_floor = FALSE;
			bool is_hit = FALSE;
			VECTOR prioritize_floor_offset_dir = VGet(0, 0, 0);// prioritizeの押し戻し方向を記憶
			for (int i = 0; i < all_poly_num; i++)
			{
				//ポリゴンを代入
				auto poly = all_poly_[i];
				//衝突しているとき
				if (next_coll->IsHitTriangle(poly->Position[0], poly->Position[1], poly->Position[2]) ||
					(HitCheck_Line_Triangle(old_coll->GetPos(), next_coll->GetPos(), poly->Position[0], poly->Position[1], poly->Position[2]).HitFlag) == 1)
				{
					is_projection = FALSE;
					//中点を出す
					VECTOR poly_center_pos =
						VGet((poly->Position[0].x + poly->Position[1].x + poly->Position[2].x) / 3,
							(poly->Position[0].y + poly->Position[1].y + poly->Position[2].y) / 3,
							(poly->Position[0].z + poly->Position[1].z + poly->Position[2].z) / 3
						);

					//今のポリゴンがprioritize_floor_numの範囲内だと
					if (i < prioritize_floor_num_)
					{

						// normの記憶
						is_hit_prioritize_floor = TRUE;

						//90,-90度回転させた
						VECTOR patern_a = VNorm(VectorAssistant::VGetRotRadZ(poly->Normal, 90));		// 90度回転
						VECTOR patern_b = VNorm(VectorAssistant::VGetRotRadZ(poly->Normal, -90));		// -90度回転

						float petern_a_dot = VDot(VNorm(offset_vel), patern_a);					// 90度回転したvectorのdot
						float petern_b_dot = VDot(VNorm(offset_vel), patern_b);					// -90度回転したvectorのdot
						prioritize_floor_offset_dir = (petern_b_dot > petern_a_dot) ? patern_b : patern_a;

					}

					/*----------ここからはセグメントのやつ(capsuleのstart_posのやつ)------------*/
					
					offset_vel = GetSegmentPolySuckVel(old_pos, next_pos, poly_center_pos, poly->Normal);
					offset_vel = VAdd(offset_vel, VScale(poly->Normal, old_coll->GetRadius()));
					//offset分足したカプセルの座標
					next_coll = old_coll->Clone();
					next_coll->Update(offset_vel);
					next_pos = next_coll->GetPos();
					capsule_end_pos = next_coll->GetCenterPos();

					// 当たり判定検出の位置を更新
					next_to_old_cap_ = std::make_shared<CollisionCapsule>(capsule_start_pos, capsule_end_pos, coll_radius);

					// 移動後にもう一度何かと当たっているのかを調べる
					for (int j = 0; j < all_poly_num; j++)
					{
						poly = all_poly_[j];

						if (next_coll->IsHitTriangle(poly->Position[0], poly->Position[1], poly->Position[2])
							|| (HitCheck_Line_Triangle(old_pos, next_pos, poly->Position[0], poly->Position[1], poly->Position[2]).HitFlag) == 1)
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
		
		
		
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hit_dim);
	}

	return offset_vel;
}