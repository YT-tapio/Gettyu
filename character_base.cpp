#include"object_base.h"
#include"collision_base.h"
#include"animation.h"
#include"stage.h"
#include"character_base.h"
#include"vector_assistant.h"

#include"FPS.h"

CharacterBase::CharacterBase(Stage* stage, std::shared_ptr<CollisionBase> coll, const VECTOR& pos, const VECTOR& rot, const VECTOR& scale, const int model_handle)
	: ObjectBase(pos, rot,scale,model_handle)
	, stage_(stage)
	, coll_(coll)
	, is_ground_(FALSE)
	, is_move_(FALSE)
	,fall_speed_(0.f)
{

}

CharacterBase::~CharacterBase()
{

}

/*----protected----*/

void CharacterBase::CheckIsGround()
{
	//stage‚ÉŽ©•ª‚Ìî•ñ‚ð“n‚µ‚Ä”»’è‚ðs‚Á‚Ä‚à‚ç‚¤
	is_ground_ = stage_->CheckDownColl(coll_);
}



void CharacterBase::SetDeltaTime()
{
	delta_time_ = FPS::GetInstance().GetDeltaTime();
}

void CharacterBase::Init()
{



}


void CharacterBase::Update()
{
	//model‚Ìmat‚ðset
	MV1SetMatrix(model_, mat_);

}

void CharacterBase::Draw()
{
	MV1DrawModel(model_);
}

void CharacterBase::Debug()
{

}

