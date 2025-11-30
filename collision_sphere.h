#pragma once

class CollisionBase;

class CollisionSphere : public CollisionBase
{
private:




public:

	CollisionSphere(const VECTOR& pos,const float r);

	~CollisionSphere()override;

	std::shared_ptr<CollisionBase> Clone() const override { return std::make_shared<CollisionSphere>(pos_, radius_); }

};