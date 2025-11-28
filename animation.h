#pragma once
#include<vector>
#include<iostream>

enum class AnimationType
{
    kNothing,
    kIdle,
    kWalk,
    kSlowRun,
    kFastRun,
    kNoLoop,        //ここより先のアニメーションはループなし
    kJumpUp,
    kJumpDown,
    kSurprise,
    kAttack,        //ここより先は攻撃アニメーション(最後になるとアニメーションを終了)
    kSwordSlash,
    kSuperAttackFirst
};


struct AnimationData
{
    AnimationType type;         // アニメーションの種類

    int model_handle;            // モデル
    int attach_index;              // アタッチの要素数
    int animation_handle;       // アニメーションの名前
    int index;                          //識別番号

    float total_time;      //総再生時間
    float play_time;       //流しているアニメーションの時間

    float play_speed;      //再生スピード
};


void Load(AnimationData& animation_data,
    const char name[], AnimationType type, int model, int ind,float play_speed);

/// <summary>
/// モデルのアタッチを行う
/// </summary>
/// <param name="model">アタッチ元のロードしたモデル名</param>
// void Attach(int model);


class Animation
{
private:

    std::vector<AnimationData> animation_data_;         //アニメーション


    float blend_rate_ = 2.0f;              //初期値を最大にする

    int model_handle_;
    AnimationType now_type_;
    AnimationType before_type_;

    int now_blend_attach_index_;
    int before_blend_attach_index_;

    bool is_blend_ = FALSE;
    bool is_play_ = FALSE;

    float delta_time_;

public:

    Animation();

    ~Animation();

    /// <summary>
    /// アニメーションを違和感なく再生する初期化
    /// </summary>
    void InitBlend(AnimationType now, AnimationType before);



    /// <summary>
    /// アニメーションを加える
    /// </summary>
    /// <param name="animtion_data"></param>
    void Add(const AnimationData& animation_data);

    /// <summary>
    /// アニメーションをアタッチ
    /// </summary>
    void Attach(AnimationType type);

    /// <summary>
    /// デタッチ
    /// </summary>
    void Detach(AnimationType type);

    /// <summary>
    /// アニメーションを違和感なく変更させる処理更新
    /// </summary>
    void BlendUpdate();

    /// <summary>
    /// モデルのアタッチを行う
    /// </summary>
    /// <param name="name[]">アタッチ元のモデル名</param>
    void Update(AnimationType type);

    /// <summary>
    /// デルタタイムの更新
    /// </summary>
    /// <param name="delta_time"></param>
    void SetDeltaTime(const float& delta_time)
    {
        delta_time_ = delta_time;
    }

    /// <summary>
    /// is_blend_1の更新
    /// </summary>
    /// <param name="flag"></param>
    void SetBlend(bool flag)
    {
        is_blend_ = flag;
    }

    void SetIsEnd(bool flag)
    {
        is_play_ = flag;
    }

    /// <summary>
    /// 変更しきったらアニメーション更新を終わらせる
    /// </summary>
    /// <returns>終わらせるのがTRUE</returns>
    bool CheckBlend() const
    {
        return blend_rate_ > 1.0f;
    }

    /// <summary>
    /// is_blend_の結果を返す
    /// </summary>
    /// <returns></returns>
    bool GetBlendFlag() const
    {
        return is_blend_;
    }

    const bool IsPlay() const { return is_play_; }

    float GetPlayTime(const AnimationType& type)
    {
        for (const auto& anim : animation_data_)
        {
            if (anim.type == type)
            {
                return anim.play_speed;
            }
        }
    }


    bool GetIsPlay(const AnimationType& type)
    {
        for (const auto& anim : animation_data_)
        {
            if (anim.type == type)
            {
                if (anim.play_time < anim.total_time)
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }

    //デバッグ用
    void Draw(const AnimationType& type);
};