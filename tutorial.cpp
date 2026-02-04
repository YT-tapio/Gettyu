#include<memory>
#include"DxLib.h"
#include"tutorial.h"
#include"vector_assistant.h"

Tutorial::Tutorial()
{

}

bool IsInSight(const VECTOR& my_pos, const VECTOR& other_pos, const float& r)
{
	VECTOR dist = VSub(other_pos, my_pos);

	return (VSize(dist) < r);
}

void Tutorial::Awake()
{
	//当たり判定の場所
	weapon_coll_pos_		= VectorAssistant::GetZeroVec();			// 武器切り替え
	attack_coll_pos_		= VectorAssistant::GetZeroVec();			// 攻撃方法
	super_attack_coll_pos_	= VectorAssistant::GetZeroVec();			// 必殺技

	// 当たり判定を生成
	weapon_info_coll_r_			= 30.f;
	attack_info_coll_r_			= 30.f;
	super_attack_info_coll_r_	= 30.f;

	// タイマーを生成
	const float kDispTimerMax		= 5.f;
	weapon_info_timer_				= std::make_shared<ConditionTimer>(kDispTimerMax);
	attack_info_timer_				= std::make_shared<ConditionTimer>(kDispTimerMax);
	super_attack_info_timer_		= std::make_shared<ConditionTimer>(kDispTimerMax);

	is_disp_weapon_coll_info_		= FALSE;
	is_disp_attack_coll_info_		= FALSE;
	is_disp_super_attack_coll_info_ = FALSE;

	// 文字を描画する際のポジション
	weapon_info_pos_		= VectorAssistant::GetZeroVec();
	attack_info_pos_		= VectorAssistant::GetZeroVec();
	super_attack_info_pos_	= VectorAssistant::GetZeroVec();

}

void Tutorial::Reset()
{
	// タイマーのリセット

	weapon_info_timer_->Reset();
	weapon_info_timer_->Reset();
	weapon_info_timer_->Reset();

	is_disp_weapon_coll_info_ = FALSE;
	is_disp_attack_coll_info_ = FALSE;
	is_disp_super_attack_coll_info_ = FALSE;

}

void Tutorial::CheckCollision(const VECTOR& pos)
{
	
	
	if (!is_disp_weapon_coll_info_)
	{
		is_disp_attack_coll_info_ = IsInSight(weapon_coll_pos_, pos, weapon_info_coll_r_);
	}

	if (!is_disp_attack_coll_info_)
	{
		is_disp_attack_coll_info_ = IsInSight(attack_coll_pos_,pos,attack_info_coll_r_);
	}


	if (!is_disp_super_attack_coll_info_)
	{
		is_disp_attack_coll_info_ = IsInSight(super_attack_coll_pos_, pos, super_attack_info_coll_r_);
	}

}


void Tutorial::Draw()
{
	// ここで文字を描画する

}

void Tutorial::Debug()
{
	DrawSphere3D(weapon_coll_pos_, weapon_info_coll_r_, 20, GetColor(255, 255, 255), GetColor(255, 255, 255),FALSE);
	DrawSphere3D(attack_coll_pos_, attack_info_coll_r_, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	DrawSphere3D(super_attack_coll_pos_, super_attack_info_coll_r_, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
}