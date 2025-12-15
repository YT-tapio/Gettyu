#pragma once
#include<iostream>
#include"DxLib.h"

//enumを作る
enum class SituationName
{
    kNothing,               // 何もない
    kStandBy,               // ゲーム開始前
    kGet,                   // ゲット時
    kAttack,                // 攻撃を受ける
    kPerformance,           // 演出
    kSuperAttack,           // 必殺中
    kVacuum,                // 吸引中
    kClearOffset,
    kClear
};

//ゲームの今起きている現象を管理する
class Situation
{
private:

    //変数を持たせる
    SituationName now_situation_ = SituationName::kNothing;

    //相手を捕まえた時のポジションを保持
    VECTOR get_situation_pos_ = VGet(0.f,0.f,0.f);

    //何番目の敵を捕まえたかの記憶
    int rem_num_ = 0;

    // コンストラクタを非公開にする
    Situation() {}


public:

    // インスタンスを取得するためのメソッド
    static Situation& GetInstance()
    {
        static Situation instance; // 静的変数としてインスタンスを定義
        return instance;
    }
    // コピーコンストラクタと代入演算子を削除
    Situation(const Situation&) = delete;
    Situation& operator=(const Situation&) = delete;
    
    void Init()
    {
        now_situation_ = SituationName::kNothing;
    }

    /// <summary>
    /// 今の状況をセット
    /// </summary>
    /// <param name="num"></param>
    void SetSituationName(SituationName num)
    {
        now_situation_ = num;
    }

    /// <summary>
    /// ゲット時のポジションをセットする
    /// </summary>
    /// <param name="pos"></param>
    void SetGetSituationPos(const VECTOR& pos)
    {
        get_situation_pos_ = pos;
    }

	void SetRemNum(const int num)
	{
		rem_num_ = num;
	}

    const SituationName GetSituationName() const { return now_situation_; }


    const VECTOR GetSituationPos() const { return get_situation_pos_; }

	const int GetRemNum() const { return rem_num_; }

};
