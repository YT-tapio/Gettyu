#pragma once
#include"base_object.h"

struct CapsuleData;
class Player;
class WayPoint;

class Stage : public ObjectBase
{
private:

	static const int kMaxHitColl = 2048;	// 処理するコリジョンポリゴンの最大数
	static constexpr int	kHitTryNum = 16;		// 壁押し出し処理の最大試行回数
	static constexpr float	kHitSlideLength = 5.0f;		// 一度の壁押し出し処理でスライドさせる距離

	std::vector<std::shared_ptr<WayPoint>> way_points_;

	VECTOR scale_;	//モデルの大きさ


	// HACK: 壁はXZ平面に垂直である前提で成り立っている。それ以外を置くとバグる
	int							wall_num_;			// 壁ポリゴンと判断されたポリゴンの数
	int							floor_num_;			// 床ポリゴンと判断されたポリゴンの数

	int before_hit_num_ = 0;

	MV1_COLL_RESULT_POLY* wall_[kMaxHitColl];	// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* floor_[kMaxHitColl];	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列

	CapsuleData next_to_old_cap_;


	// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断し、保存する
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hit_dim, const VECTOR& check_position);

	void MakeCollCheckCapsule(CapsuleData old_cap, CapsuleData next_cap);

	// 壁ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
	VECTOR CheckHitWithWall(Player& player, const VECTOR& check_position);

	// 床ポリゴンとの当たりをチェックし、補正すべき移動ベクトルを返す
	VECTOR CheckHitWithFloor(Player& player, const VECTOR& check_position);

	// 壁or床の情報を受け取って調整したposを返す
	VECTOR CheckEntityCollisionFixedPos(Player& player, MV1_COLL_RESULT_POLY* entity, int hit_num, CollisionData& old_cap, CollisionData& future_cap);

public:

	Stage(int model_handle, VECTOR pos, float scale);

	~Stage();

	void Draw() override;

	void Debug() override;

	VECTOR CheckEnemyCollision(EnemyBase* enemy, const VECTOR& velocity);

	VECTOR CheckCollision(Player& player, const VECTOR& velocity);



};