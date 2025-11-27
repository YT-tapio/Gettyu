#include"object_base.h"
#include"animation.h"
#include"stage.h"
#include"character_base.h"
#include"vector_assistant.h"



CharacterBase::CharacterBase(Stage* stage, const VECTOR& pos, const VECTOR& rot, const VECTOR& scale, const int model_handle)
	:ObjectBase(pos, rot,scale,model_handle)
	,stage_(stage)
{

}

CharacterBase::~CharacterBase()
{

}

/*----protected----*/

bool CharacterBase::IsOnGround()
{
	//stage‚É©•ª‚Ìî•ñ‚ğ“n‚µ‚Ä”»’è‚ğs‚Á‚Ä‚à‚ç‚¤
	return TRUE;
}



void CharacterBase::Init()
{



}


void CharacterBase::Update()
{
	//model‚Ìmat‚ğset
	MV1SetMatrix(model_, mat_);

}

void CharacterBase::Draw()
{
	MV1DrawModel(model_);
}

void CharacterBase::Debug()
{

}

