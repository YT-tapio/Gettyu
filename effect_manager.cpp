#include"effect_manager.h"
#include"EffekseerForDXLib.h"
#include "DxLib.h"


// コンストラクタ
EffectManager::EffectManager()
    : effectResourceHandle(-1)
    , playingEffectHandle(-1)
    , playCount(0)
    , on_disp_(TRUE)
    ,play_type_(EffectPlayType::kStart)
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
    DeleteEffekseerEffect(effectResourceHandle);
}

// 初期化
void EffectManager::Initialize()
{
    // DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
    // Effekseerを使用するには必ず設定する。
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    // 引数には画面に表示する最大パーティクル数を設定する。
    if (Effkseer_Init(EffectParticleLimit) == -1) { DxLib_End(); }

    // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
    // Effekseerを使用する場合は必ず設定する。
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    // DXライブラリのデバイスロストした時のコールバックを設定する。
    // ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();

    // Zバッファを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetUseZBuffer3D(TRUE);

    // Zバッファへの書き込みを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetWriteZBuffer3D(TRUE);
}

// 読み込み
void EffectManager::Load()
{
    // エフェクトのリソースを読み込む
    effectResourceHandle = LoadEffekseerEffect(EffectFilePath, EffectSize);

    if (effectResourceHandle == -1)
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
    if (playCount > EffectPlayInterval)
    {
        
        if (FALSE)
        {
            // エフェクトを再生する。
            playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
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
        playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
        play_type_ = kPlay;
    }
    
    if (play_type_ == EffectPlayType::kEnd)
    {
        StopEffekseer3DEffect(playingEffectHandle);
    }

    // 再生カウントを進める
    playCount++;

    if (TRUE)
    {
        // 再生中のエフェクトを移動する。
        SetPosPlayingEffekseer3DEffect(playingEffectHandle, playPosition.x, playPosition.y, playPosition.z);
    }
    else
    {
        // 再生中のエフェクトを移動する。
        SetPosPlayingEffekseer3DEffect(playingEffectHandle, 0, 0, 0);
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