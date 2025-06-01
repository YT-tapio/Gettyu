#include"effect.h"
#include"EffekseerForDXLib.h"
#include "DxLib.h"


// コンストラクタ
EffectManager::EffectManager()
    : effectResourceHandle(-1)
    , playingEffectHandle(-1)
    , playCount(0)
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
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playPosition">再生座標</param>
void EffectManager::Update(VECTOR playPosition)
{
    // 定期的にエフェクトを再生する
    if (!(playCount % EffectPlayInterval))
    {
        // エフェクトを再生する。
        playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
    }

    // 再生カウントを進める
    playCount++;

    if (FALSE)
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