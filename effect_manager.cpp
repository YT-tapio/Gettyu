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
        //printfDx("effect_managerの読み込み失敗");
    }
    //playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playPosition">再生座標</param>
void EffectManager::Update()
{
    Effekseer_Sync3DSetting();
    // Effekseerにより再生中のエフェクトを更新する。
    UpdateEffekseer3D();
}

// 描画
void EffectManager::Draw()
{
    // Effekseerにより再生中のエフェクトを描画する。
    DrawEffekseer3D();
    
}