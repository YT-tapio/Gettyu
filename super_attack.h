#pragma once


class SuperAttack
{
private:

	//場面変わりが何個あるか
	const int kSwitchSituationNumMax = 4;

	int now_situation_num_ = 0;

	// エフェクト(座標とパス)
	VECTOR pos_;
	int effect_handle_;


	float delta_time_;

	// 再生
	bool is_play_;


	//カウントするやつ
	float play_count_;
	float max_play_count_;

public:


	SuperAttack(const VECTOR& pos,int effect_handle);


	~SuperAttack();


	void Update();


	void SetNowSituatuin(int num) { now_situation_num_ = num; }


	void SetPos(const VECTOR& pos) { pos_ = pos; }


	void SetIsPlay(bool is_play) { is_play_ = is_play; }


	const int GetNowSituation() const { return now_situation_num_; }

};


///モーションブラー