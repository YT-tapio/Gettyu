#include<iostream>
#include"collision_base.h"
#include"collision_sphere.h"


CollisionSphere::CollisionSphere(const VECTOR& pos,const float r)
	: CollisionBase(pos,CollisionName::kSphere,r)
{

}

CollisionSphere::~CollisionSphere()
{

}


