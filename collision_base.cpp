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

