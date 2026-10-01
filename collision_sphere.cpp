#include<iostream>
#include"collision_base.h"
#include"collision_sphere.h"


CollisionSphere::CollisionSphere(const VECTOR& pos,const float r)
	: ColliderBase(pos,CollisionName::kSphere,r)
{

}

CollisionSphere::~CollisionSphere()
{

}


MV1_COLL_RESULT_POLY_DIM CollisionSphere::GetCollInfo(const int model)
{
	return MV1CollCheck_Sphere(model, -1, pos_, radius_);
}

bool CollisionSphere::IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3)
{
	return HitCheck_Sphere_Triangle(pos_, radius_, tri_1, tri_2, tri_3);
}