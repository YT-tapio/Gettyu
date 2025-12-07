#pragma once
#include"effect.h"
#include"condition_timer.h"
#include"super_attack_state.h"

class SuperAttack
{
private:

	//場面変わりが何個あるか
	const int kSwitchSituationNumMax = 4;

	const float kCoolTimeMax	= 20.f;
	const float kActiveTimeMax = 10.f;
	const float kOffsetTimeMax = 4.f;

	//必殺技のクールタイムや効果時間の示し
	float skill_num_;

	Effect* effect_;
	Effect* effect_start_;
	Effect* effect_end_;

	std::shared_ptr<ConditionTimer> cool_time_;
	std::shared_ptr<ConditionTimer> active_time_;
	std::shared_ptr<ConditionTimer> offset_time_;

	SuperAttackState state_;

	int now_situation_num_ = 0;

	// エフェクト(座標とパス)
	VECTOR effect_pos_;
	VECTOR effect_start_pos_;
	VECTOR effect_end_pos_;
	int effect_handle_;
	int play_handle_;		//再生するときの箱
	

	float delta_time_;

	// 再生
	bool is_play_;
	bool is_ready_;
	bool is_active_;
	bool is_offset_;

	void CoolTimeUpdate();

	void OffsetUpdate();

	void ActiveUpdate();

public:


	SuperAttack(const VECTOR& pos, const char* file_path);


	~SuperAttack();

	void Init();

	void Update();

	void EffectUpdate();

	

	void SetNowSituatuin(int num) { now_situation_num_ = num; }


	void SetPos(const VECTOR& pos, const VECTOR& pos2)
	{ 
		effect_pos_ = pos;
		effect_start_pos_ = pos2;
		effect_end_pos_ = pos2;
	}

	
	void SetDeltaTime(const float& delta_time) 
	{ 
		effect_->SetDeltaTime(delta_time);
		effect_start_->SetDeltaTime(delta_time);
		effect_end_->SetDeltaTime(delta_time);
	}


	void SetIsPlay(bool is_play) { is_play_ = is_play; }


	void Draw();

	void Debug();

	/// <summary>
	/// 発動のアクションを起こしたかの判断
	/// </summary>
	/// <returns></returns>
	bool IsAction();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const int GetNowSituation() const { return now_situation_num_; }

	const VECTOR GetEffectPosition() const { return effect_pos_; }

	const bool GetEffectIsPlay() const { return effect_->GetIsPlay(); }

	const bool GetIsReady() const { return is_ready_; }

	const float GetEffectPlayCount() const { return effect_end_->GetPlayCount(); }

	const SuperAttackState GetState() const { return state_; }

};


///モーションブラー