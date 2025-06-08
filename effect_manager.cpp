#include"effect_manager.h"
#include"EffekseerForDXLib.h"
#include "DxLib.h"


// コンストラクタ
EffectManager::EffectManager(const char* file_path,float size,int play_interval)
    : resource_handle_(-1)
    , playing_handle_(-1)
    , playCount(0)
    , on_disp_(TRUE)
    ,play_type_(EffectPlayType::kStart)
    ,delta_time_(0.0f)
    ,file_path_(file_path)
    ,size_(size)
    ,play_interval_(play_interval)
{
    // 初期化
    Initialize();

    // 読み込み
    Load();
}

// デストラクタ
EffectManager::~EffectManager()
{
    // エフェクトリソースの開放
    // (Effekseer終了時に破棄されるので削除しなくてもいい)
    DeleteEffekseerEffect(resource_handle_);
}

// 初期化
void EffectManager::Initialize()
{
    
}

// 読み込み
void EffectManager::Load()
{
    // エフェクトのリソースを読み込む
    resource_handle_ = LoadEffekseerEffect(file_path_, size_);

    if (resource_handle_ == -1)
    {
        printfDx("失敗");
    }
    //playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playPosition">再生座標</param>
void EffectManager::Update(const VECTOR& playPosition)
{

    if (!on_disp_)
    {
        return;
    }
    

    // 定期的にエフェクトを再生する
    if (playCount > play_interval_)
    {
        
        if (FALSE)
        {
            // エフェクトを再生する。
            playing_handle_ = PlayEffekseer3DEffect(resource_handle_);
        }
        /*
        if (playingEffectHandle == -1)
        {
            printfDx("失敗");
        }
        */

        play_type_ = EffectPlayType::kEnd;
        on_disp_ = FALSE;
        playCount = 0;

    }

    if (play_type_ == EffectPlayType::kStart)
    {
        playing_handle_ = PlayEffekseer3DEffect(resource_handle_);
        play_type_ = kPlay;
    }
    
    if (play_type_ == EffectPlayType::kEnd)
    {
        StopEffekseer3DEffect(playing_handle_);
    }

    // 再生カウントを進める
    playCount += (1 * (delta_time_ * 10));

    if (TRUE)
    {
        // 再生中のエフェクトを移動する。
        SetPosPlayingEffekseer3DEffect(playing_handle_, playPosition.x, playPosition.y, playPosition.z);
    }
    else
    {
        // 再生中のエフェクトを移動する。
        SetPosPlayingEffekseer3DEffect(playing_handle_, 0, 0, 0);
    }
    
   

    // Effekseerにより再生中のエフェクトを更新する。
    UpdateEffekseer3D();
}

// 描画
void EffectManager::Draw()
{
    // Effekseerにより再生中のエフェクトを描画する。
    DrawEffekseer3D();
    
}