#pragma once
#include"collision_data.h"

class CollisionBase
{
private:
	
	CollisionName name_;

protected:

	VECTOR pos_;
	float radius_;

public:

	CollisionBase(const VECTOR& pos,const CollisionName& name, const float& r);

	virtual ~CollisionBase();

	virtual void Update(const VECTOR& vel);

	/// @brief @brief カプセルに当たっているモデルの情報を返す
	/// @param model モデル
	/// @return 当たっているポリゴンの情報
	virtual MV1_COLL_RESULT_POLY_DIM GetCollInfo(const int model);

	/// @brief 三角形との当たり判定
	/// @param tri_1 
	/// @param tri_2 
	/// @param tri_3 
	/// @return TRUE : 当たっている
	virtual bool IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3);

	const float GetRadius() const { return radius_; }

	const CollisionName GetName() const { return name_; }
	
	const VECTOR GetPos() const { return pos_; }

	

};