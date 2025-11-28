#include"collision_base.h"


CollisionBase::CollisionBase(const VECTOR& pos,const CollisionName& name,const float& r)
	: pos_(pos)
	, name_(name)
	, radius_(r)
{

}


CollisionBase::~CollisionBase()
{

}

void CollisionBase::Update(const VECTOR& vel)
{
	pos_ = VAdd(pos_, vel);
}

void CollisionBase::Debug()
{
	DrawSphere3D(pos_, radius_, kDivNum, kDebugColor, kDebugColor, FALSE);
}

float CollisionBase::GetWidth()
{
	return radius_;
}


VECTOR CollisionBase::GetCenterPos()
{
	VECTOR center_pos = pos_;
	return center_pos;
}

MV1_COLL_RESULT_POLY_DIM CollisionBase::GetCollInfo(const int model)
{
	return MV1CollCheck_Sphere(model, -1, pos_, radius_);
}

bool CollisionBase::IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3)
{
	return HitCheck_Sphere_Triangle(pos_, radius_, tri_1, tri_2, tri_3);
}

