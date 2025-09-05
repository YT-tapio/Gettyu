#pragma once
#include"effect.h"

class SuperAttack
{
private:

	//場面変わりが何個あるか
	const int kSwitchSituationNumMax = 4;


	Effect* effect_;


	int now_situation_num_ = 0;

	// エフェクト(座標とパス)
	VECTOR pos_;
	int effect_handle_;
	int play_handle_;		//再生するときの箱
	

	float delta_time_;

	// 再生
	bool is_play_;


	//カウントするやつ
	float play_count_;
	float max_play_count_;

public:


	SuperAttack(const VECTOR& pos, const char* file_path);


	~SuperAttack();


	void Update();


	void SetNowSituatuin(int num) { now_situation_num_ = num; }


	void SetPos(const VECTOR& pos) 
	{ 
		pos_ = pos;
	}


	void SetDeltaTime(const float& delta_time) { effect_->SetDeltaTime(delta_time); }


	void SetIsPlay(bool is_play) { is_play_ = is_play; }


	void Draw();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const int GetNowSituation() const { return now_situation_num_; }

	const VECTOR GetEffectPosition() const { return pos_; }

	const bool GetEffectIsPlay() const { return effect_->GetIsPlay(); }

};


///モーションブラー