#include"DxLib.h"
#include<iostream>
#include<vector>
#include"animation.h"

void Load(AnimationData& animation_data,
    const char name[], AnimationType type, int model, int ind,float play_speed)
{
    animation_data.type = type;
    animation_data.animation_handle = MV1LoadModel(name);

    if (animation_data.animation_handle == -1)
    {
        printfDx("アニメーションの読み込みに失敗してます");
    }

    animation_data.model_handle = model;
    animation_data.index = ind;
    animation_data.play_time = 0.0f;
    animation_data.total_time = 0.0f;
    animation_data.play_speed = play_speed;

}


Animation::Animation()
    : model_handle_(-1)
    , now_type_(AnimationType::kNothing)
    , before_type_(AnimationType::kNothing)
    , now_blend_attach_index_(-1)
    , before_blend_attach_index_(-1)
    , delta_time_(0.0f)
{

}


Animation::~Animation()
{
    //なし
    //アニメーションのデータ解放
    for (auto& anim : animation_data_)
    {
        MV1DeleteModel(anim.animation_handle);
    }

}

void Animation::InitBlend(AnimationType now, AnimationType before)
{
    now_type_ = now;

    before_type_ = before;

    for (auto& animation : animation_data_)
    {
        if (before == animation.type)
        {
            before_blend_attach_index_ = animation.attach_index;
        }

        if (now == animation.type)
        {
            now_blend_attach_index_ = animation.attach_index;
        }

        model_handle_ = animation.model_handle;
    }

    blend_rate_ = 0.0f;
}

void Animation::Add(const AnimationData& animation_data)
{
    animation_data_.push_back(animation_data);
}


void Animation::Attach(AnimationType type)
{
    for (auto& animation : animation_data_)
    {
        if (type == animation.type)
        {
            animation.attach_index =
                MV1AttachAnim(animation.model_handle, animation.index, animation.animation_handle, FALSE);

            animation.total_time =
                MV1GetAttachAnimTotalTime(animation.model_handle, 
                    animation.attach_index);
            
            animation.play_time = 0.f;

            if (animation.attach_index == -1)
            {
                printfDx("アタッチに失敗しました");
            }

            if (animation.total_time < 0)
            {
                printfDx("トータルおかしい");
            }

            break;
        }
    }
}


void Animation::Detach(AnimationType type)
{
    for (auto& animation : animation_data_)
    {
        if (type == animation.type)
        {
            animation.play_time = 0.0f;
            MV1DetachAnim(animation.model_handle, animation.attach_index);
            animation.attach_index = -1;
        }

    }
}

void Animation::BlendUpdate()
{

    if (GetBlendFlag())
    {
        
        for (auto& animation : animation_data_)
        {
            if (before_type_ == animation.type)
            {
                MV1SetAttachAnimBlendRate(animation.model_handle
                    , animation.attach_index, 1.0f - blend_rate_);
            }

        }

        for (auto& animation : animation_data_)
        {
            if (now_type_ == animation.type)
            {
                MV1SetAttachAnimBlendRate(animation.model_handle,
                    animation.attach_index, blend_rate_);
            }
        }

        
        blend_rate_ += (delta_time_ * 0.5f);

        if (blend_rate_ > 1.0f)
        {
            SetBlend(FALSE);
            Detach(before_type_);

            //printfDx("Detach");

        }

    }
    else
    {

        for (auto& animation : animation_data_)
        {
            if (now_type_ == animation.type)
            {
                MV1SetAttachAnimBlendRate(animation.model_handle,
                    animation.attach_index, 1.f);
            }
        }
        
        blend_rate_ = 0.0f;
    }
}

void Animation::Update(AnimationType type)
{
    for (auto& animation : animation_data_)
    {

        if (type == animation.type)
        {
            animation.play_time += (animation.play_speed * delta_time_);
            is_play_ = TRUE;

            if (animation.play_time >= animation.total_time)
            {

                
                if (animation.type < AnimationType::kNoLoop)
                {
                    animation.play_time = 0.0f;
                }
                else
                {
                    if (animation.type > AnimationType::kAttack)
                    {
                        animation.play_time = animation.total_time - 0.1f;
                        is_play_ = FALSE;
                    }
                    else
                    {
                        animation.play_time = animation.total_time;
                        SetIsEnd(TRUE);
                    }
                    
                }
                
                
            }

            MV1SetAttachAnimTime(animation.model_handle, animation.attach_index,
                animation.play_time);

        }
    }

    BlendUpdate();
}

void Animation::Debug(const AnimationType& type)
{
    for (auto& animation : animation_data_)
    {
        if (animation.type == type)
        {
            DrawFormatString(400, 100, GetColor(0, 0, 0), "%.3f", animation.play_time);

            break;
        }
    }
}