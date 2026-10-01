#pragma once

class ColliderBase;

class CollisionSphere : public ColliderBase
{
private:




public:

	CollisionSphere(const VECTOR& pos,const float r);

	~CollisionSphere()override;

	std::shared_ptr<ColliderBase> Clone() const override { return std::make_shared<CollisionSphere>(pos_, radius_); }

	MV1_COLL_RESULT_POLY_DIM GetCollInfo(const int model) override;

	bool IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3) override;

};