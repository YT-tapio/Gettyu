#pragma once

class ColliderBase;

class CollisionCapsule : public ColliderBase
{
private:

	VECTOR end_pos_;

public:

	CollisionCapsule(const VECTOR& pos, const VECTOR& end_pos, const float& r);

	~CollisionCapsule() override;

	void Update(const VECTOR& vel) override;

	void Debug() override;

	std::shared_ptr<ColliderBase> Clone() const override 
	{
		return std::make_shared < CollisionCapsule >(pos_, end_pos_, radius_);
	}

	float GetWidth() override;

	VECTOR GetCenterPos() override;

	MV1_COLL_RESULT_POLY_DIM GetCollInfo(const int model) override;

	bool IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3) override;
};