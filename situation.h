#pragma once
#include<iostream>
#include"DxLib.h"

//enumを作る
enum class SituationName
{
    kNothing,       //何もない
    kGet,             //ゲット時
    kAttack          //
};

//ゲームの今起きている現象を管理する
class Situation
{
private:

    //変数を持たせる
    SituationName now_situation_;

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
    
    void SetSituation(SituationName num)
    {
        now_situation_ = num;
    }

    const SituationName GetSituation() const { return now_situation_; }

};
