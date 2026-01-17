#include"effect_manager.h"
#include"EffekseerForDXLib.h"
#include "DxLib.h"


// コンストラクタ
EffectManager::EffectManager(const char* file_path,float size,int play_interval)
    : on_disp_(TRUE)
    , play_type_(EffectPlayType::kStart)
    , delta_time_(0.0f)
    , file_path_(file_path)
    , size_(size)
    , play_interval_(play_interval)
{
    // 初期化
    Initialize();

    // 読み込み
    Load();
}

// デストラクタ
EffectManager::~EffectManager()
{
 
}

// 初期化
void EffectManager::Initialize()
{
    
}

// 読み込み
void EffectManager::Load()
{

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