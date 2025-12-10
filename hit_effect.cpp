#include<iostream>
#include"DxLib.h"
#include"effect.h"
#include"hit_effect.h"
#include"vector_assistant.h"
#include"FPS.h"
#include"situation.h"
#include"const_rad.h"

HitEffect::HitEffect()
{
	const char* kEffectPath		= "data/effect/Pierre01/SonicBoom.efkefc";	// effectÇÃpath
	const float kEffectSpeed	= 10.f;										// effectÇÃçƒê∂ë¨ìx

	const float kEffectScale	= 1.f;
	const float kEffectCountMax = 50.f;

	pos_	= VectorAssistant::GetZeroVec();
	scale_	= VectorAssistant::GetSame3DVec(kEffectScale);
	rot_	= VGet(-(kOneRad * 90.f),0.f,0.f);

	effect_ = std::make_shared<Effect>(kEffectPath, pos_, rot_, kEffectSpeed, kEffectScale, kEffectCountMax, FALSE);

}

HitEffect::~HitEffect()
{

}

void HitEffect::SetDeltaTime()
{
	effect_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());
}

void HitEffect::Update()
{
	if (Situation::GetInstance().GetSituationName() == SituationName::kAttack)
	{
		effect_->SetPos(Situation::GetInstance().GetSituationPos());
		effect_->Play();
	}
	else
	{
		effect_->SetIsPlay(FALSE);
	}
}