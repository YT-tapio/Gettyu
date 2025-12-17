#pragma once

class ColliderBase;

class CollisionSphere : public ColliderBase
{
private:




public:

	CollisionSphere(const VECTOR& pos,const float r);

	~CollisionSphere()override;

	std::shared_ptr<ColliderBase> Clone() const override { return std::make_shared<CollisionSphere>(pos_, radius_); }

};