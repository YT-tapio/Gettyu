#pragma once
#include"object_base.h"
#include<vector>
struct CapsuleData;
class Player;
class WayPoint;
class ColliderBase;
const int kVertex = 3;

struct PolyVertexPos
{
	VECTOR pos[kVertex];
};

struct PrioritizeFloorPolyData
{
	VECTOR norm;
	VECTOR x_angle_norm;
	VECTOR z_angle_norm;
	VECTOR center_pos;
};

class Stage : public ObjectBase
{
private:

	static const int kMaxHitColl = 2048;	// 処理するコリジョンポリゴンの最大数
	static constexpr int	kHitTryNum = 16;		// 壁押し出し処理の最大試行回数
	static constexpr float	kHitSlideLength = 5.0f;		// 一度の壁押し出し処理でスライドさせる距離

	//std::vector<std::shared_ptr<WayPoint>> way_points_;

	//VECTOR scale_;	//モデルの大きさ


	// HACK: 壁はXZ平面に垂直である前提で成り立っている。それ以外を置くとバグる
	int							wall_num_;			// 壁ポリゴンと判断されたポリゴンの数
	int							floor_num_;			// 床ポリゴンと判断されたポリゴンの数
	int							prioritize_floor_num_;			// 床ポリゴンと判断されたポリゴンの数

	int before_hit_num_ = 0;
	MV1_COLL_RESULT_POLY* prioritize_floor_[kMaxHitColl];	// 優先される床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* wall_[kMaxHitColl];	// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* floor_[kMaxHitColl];	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* all_poly_[kMaxHitColl];
	std::shared_ptr<ColliderBase> next_to_old_cap_;

	std::vector<PolyVertexPos> prioritize_floor_polys_;
	std::vector<PolyVertexPos> floor_polys_;
	std::vector<PolyVertexPos> wall_polys_;
	std::vector<PrioritizeFloorPolyData> prioritize_floor_poly_data_;
	

	// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断し、保存する
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hit_dim, const VECTOR& check_position);

	void MakeCollCheckCapsule(CapsuleData old_cap, CapsuleData next_cap);

	/// <summary>
	/// セグメントと三角形(ポリゴン)の押し戻しを行い押し戻した後の移動量を返す
	/// </summary>
	/// <param name="start_pos">始点</param>
	/// <param name="end_pos"></param>
	/// <param name="poly_center_pos">ポリゴンの中心座標</param>
	/// <param name="poly_norm">法線</param>
	/// <returns></returns>
	VECTOR GetSegmentPolySuckVel(const VECTOR& start_pos, const VECTOR& end_pos, const VECTOR& poly_center_pos, const VECTOR& poly_norm);

	/// <summary>
	/// 階段かどうかの判別
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="entity_pos"></param>
	/// <param name="hit_dim"></param>
	/// <returns></returns>
	bool IsStair(const VECTOR& pos, const VECTOR& entity_pos, const float& r);

	/// @brief 平らなのかの判定を行う
	/// @param norm 
	/// @return 
	bool IsFlat(const VECTOR& norm);

	// 壁ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
	VECTOR CheckHitWithWall(Player& player, const VECTOR& check_position);

	// 床ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
	VECTOR CheckHitWithFloor(Player& player, const VECTOR& check_position);

	// 壁or床の情報を受け取って調整したposを返す
	VECTOR CheckEntityCollisionOffsetVelocity(MV1_COLL_RESULT_POLY* entity, int hit_num, std::shared_ptr<ColliderBase> obj_coll, const VECTOR& velocity);

public:

	Stage(const char* path, VECTOR pos, float scale);

	~Stage() override;

	void Init() override;

	void Update()override;

	void Draw() override;

	void Debug() override;

	/// <summary>
	/// 少し下に下げた時にあたっているかのcheck
	/// </summary>
	/// <param name="coll"></param>
	/// <returns>当たっているときTRUE</returns>
	bool CheckDownColl(std::shared_ptr<ColliderBase> coll);

	//VECTOR CheckEnemyCollision(EnemyBase* enemy, const VECTOR& velocity);

	VECTOR CheckCollision(std::shared_ptr<ColliderBase> object_coll,const VECTOR& velocity);



};