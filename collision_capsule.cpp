#include"collision_base.h"
#include"collision_capsule.h"
#include"vector_assistant.h"
#include"debug.h"

CollisionCapsule::CollisionCapsule(const VECTOR& pos,const VECTOR& end_pos,const float& r)
	:ColliderBase(pos,CollisionName::kCapsule,r)
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

void CollisionCapsule::Debug()
{
	DrawCapsule3D(pos_, end_pos_, radius_, kDivNum, kDebugColor, kDebugColor, FALSE);
	DrawFormatString(0, Debug::GetInstance().GetCurrentNum() * Debug::GetInstance().GetFontSize(), GetColor(255, 255, 255), "/*----capsule---*/");
	Debug::GetInstance().Add();
	Debug::GetInstance().VectorDraw(pos_);
	Debug::GetInstance().VectorDraw(end_pos_);
}



float CollisionCapsule::GetWidth()
{
	float width = 0.f;

	VECTOR start_to_end = VSub(end_pos_, pos_);
	float size = VSize(start_to_end);
	width = (size * 0.5f);
	width = width + radius_;

	return width;
}

VECTOR CollisionCapsule::GetCenterPos()
{
	VECTOR center_pos = VectorAssistant::GetHerf(VAdd(pos_, end_pos_));
	
	return center_pos;
}

MV1_COLL_RESULT_POLY_DIM CollisionCapsule::GetCollInfo(const int model)
{
	return MV1CollCheck_Capsule(model, -1, pos_, end_pos_, radius_);
}

bool CollisionCapsule::IsHitTriangle(const VECTOR& tri_1, const VECTOR& tri_2, const VECTOR& tri_3)
{
	return HitCheck_Capsule_Triangle(pos_, end_pos_, radius_, tri_1, tri_2, tri_3);
}