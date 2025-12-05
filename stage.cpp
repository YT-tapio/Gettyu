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

Stage::Stage(const char* path, VECTOR pos, float scale)
	: ObjectBase(pos, VectorAssistant::GetZeroVec(), VGet(scale, scale, scale), path)
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


VECTOR Stage::CheckEntityCollisionFixedPos(Player& player, MV1_COLL_RESULT_POLY* entity, int hit_num,CollisionData& old_cap,CollisionData& future_cap)
{

	


	return VGet(0, 0, 0);

	
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


bool Stage::IsStair(const VECTOR& poly_pos, const VECTOR& entity_pos)
{
	// polyの高さがentityのposよりも小さいのなら
	return entity_pos.y > poly_pos.y;
}

bool Stage::CheckDownColl(const std::shared_ptr<CollisionBase> coll)
{
	bool flag = FALSE;				//こいつが返す
	const VECTOR kDownVel = VGet(0.f, -0.1f, 0.f);
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
			flag = TRUE;
		}

	}
	else
	{
		flag = FALSE;
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
}


void Stage::Debug()
{
	next_to_old_cap_->Debug();
}


VECTOR Stage::CheckCollision(std::shared_ptr<CollisionBase> object_coll, const VECTOR& velocity)
{
	VECTOR offset_vel = velocity;
	
	//壁に当たっているのを検知する
	bool is_hit_wall = FALSE;

	// wall_num_,floor_num_の初期化
	wall_num_	= 0;
	floor_num_	= 0;

	// 今の当たり判定は未来のカプセルのとこだけになっているので、カプセルを大ききくしたやつにする(nowとnextの合計のもの)

	//新しくこいつで当たり判定を行う
	auto old_coll	= object_coll->Clone();
	auto next_coll	= object_coll->Clone();

	next_coll->Update(offset_vel);

	//auto old_player_capsule = player.GetCapsuleData();
	//auto next_player_capsule = old_player_capsule;

	VECTOR old_pos			= old_coll->GetPos();
	VECTOR next_pos			= next_coll->GetPos();

	VECTOR capsule_start_pos	= old_coll->GetCenterPos();
	VECTOR capsule_end_pos		= next_coll->GetCenterPos();
	float coll_radius				= old_coll->GetWidth();
	//当たり判定の検出のカプセルを作る
	next_to_old_cap_ = std::make_shared<CollisionCapsule>(capsule_start_pos, capsule_end_pos, coll_radius);

	// HACK: ステージポリゴンが複数ある場合、ここが繰り返し処理になる
	{

		

		// プレイヤーの周囲にあるステージポリゴンを取得する
		// ( 検出する範囲は移動距離も考慮する )
		auto hit_dim = next_to_old_cap_->GetCollInfo(model_);


		//playerが動いていない場合も考えたい	
		
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

		//壁と床に分けるのでそれを変えます


		

		//ここからが当たり判定
		for (int k = 0; k < kHitTryNum; k++)
		{
			bool is_hit = FALSE;
			for (int i = 0; i < hit_dim.HitNum; i++)
			{
				//ポリゴンを代入
				auto poly = hit_dim.Dim[i];

				//衝突しているとき
				if (next_coll->IsHitTriangle(poly.Position[0], poly.Position[1], poly.Position[2]) ||
					(HitCheck_Line_Triangle(old_coll->GetPos(), next_coll->GetPos(), poly.Position[0], poly.Position[1], poly.Position[2]).HitFlag) == 1)
				{

					//中点を出す
					VECTOR poly_center_pos =
						VGet((poly.Position[0].x + poly.Position[1].x + poly.Position[2].x) / 3,
							(poly.Position[0].y + poly.Position[1].y + poly.Position[2].y) / 3,
							(poly.Position[0].z + poly.Position[1].z + poly.Position[2].z) / 3
						);

					// 俺的には最初で判断していいと思う
					// フラグを返す関数を作るそれがTRUEの時はそいつを除外するような感じにしたい
					// 地面時に判断するようにする
					if ((!(poly.Normal.y <= 0.f)) && !IsStair(poly_center_pos, next_coll->GetPos())) { continue; }


					/*----------ここからはセグメントのやつ(capsuleのstart_posのやつ)------------*/

					//ポリゴンの中点からの距離を見てから、そのあと正射影ベクトルを出す。
					//センターからの距離
					//カプセルの開始の位置
					VECTOR poly_to_old;			//old
					VECTOR poly_to_next;

					//正射影ベクトルを出す
					VECTOR poly_to_old_proj_vec;
					VECTOR poly_to_next_proj_vec;

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
					VECTOR offset_pos = VSub(next_pos, poly_to_next_proj_vec);
					offset_pos = VAdd(offset_pos, VScale(poly.Normal, old_coll->GetRadius()));

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
					for (int j = 0; j < hit_dim.HitNum; j++)
					{
						poly = hit_dim.Dim[j];

						if (next_to_old_cap_->IsHitTriangle(poly.Position[0], poly.Position[1], poly.Position[2])
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

	return offset_vel;
}