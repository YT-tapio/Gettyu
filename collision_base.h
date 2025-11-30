#pragma once
#include<memory>
#include"collision_data.h"

class CollisionBase
{
private:
	
	CollisionName name_;

protected:

	const int kDivNum = 20;
	const int kDebugColor = GetColor(255, 255, 255);

	VECTOR pos_;
	float radius_;

public:

	CollisionBase(const VECTOR& pos,const CollisionName& name, const float& r);

	virtual ~CollisionBase();

	virtual void Update(const VECTOR& vel);

	virtual void Debug();

	virtual float GetWidth();

	virtual VECTOR GetCenterPos();

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

	virtual std::shared_ptr<CollisionBase> Clone() const = 0;

	const float GetRadius() const { return radius_; }

	const CollisionName GetName() const { return name_; }
	
	const VECTOR GetPos() const { return pos_; }

	

};