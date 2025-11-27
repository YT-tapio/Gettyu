#include"collision_base.h"
#include"collision_capsule.h"

CollisionCapsule::CollisionCapsule(const VECTOR& pos,const VECTOR& end_pos,const float& r)
	:CollisionBase(pos,CollisionName::kSphere,r)
	,end_pos_(end_pos)
{

}

CollisionCapsule::~CollisionCapsule()
{

}


void CollisionCapsule::Update(const VECTOR& vel)
{
	pos_ = VAdd(pos_, vel);
	end_pos_ = VAdd(end_pos_, vel);
}

MV1_COLL_RESULT_POLY_DIM CollisionCapsule::GetCollInfo(const int model)
{
	return MV1CollCheck_Capsule(model, -1, pos_, end_pos_, radius_);
}

bool CollisionCapsule::IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3)
{
	return HitCheck_Capsule_Triangle(pos_, end_pos_, radius_, tri_1, tri_2, tri_3);
}