#include<iostream>
#include"collision_base.h"


ColliderBase::ColliderBase(const VECTOR& pos,const CollisionName& name,const float& r)
	: pos_(pos)
	, name_(name)
	, radius_(r)
{

}


ColliderBase::~ColliderBase()
{

}

void ColliderBase::Update(const VECTOR& vel)
{
	pos_ = VAdd(pos_, vel);
}

void ColliderBase::Debug()
{
	DrawSphere3D(pos_, radius_, kDivNum, kDebugColor, kDebugColor, FALSE);
}

float ColliderBase::GetWidth()
{
	return radius_;
}


VECTOR ColliderBase::GetCenterPos()
{
	VECTOR center_pos = pos_;
	return center_pos;
}

MV1_COLL_RESULT_POLY_DIM ColliderBase::GetCollInfo(const int model)
{
	return MV1CollCheck_Sphere(model, -1, pos_, radius_);
}

bool ColliderBase::IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3)
{
	return HitCheck_Sphere_Triangle(pos_, radius_, tri_1, tri_2, tri_3);
}

