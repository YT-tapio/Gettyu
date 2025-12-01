#include"scene_manager.h"
#include"title.h"
#include"game.h"
#include"result.h"
#include"debug.h"
#include"input.h"

SceneManager::SceneManager()
{

    SetGraphMode(kGameWidth, kGameHeight, 32);			//ウィンドウのサイズとカラーモードを決める
    ChangeWindowMode(TRUE);				//ウィンドウモードにする
    if (DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
    {
        return;        // エラーが起きたら直ちに終了
    }


    // DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
    // Effekseerを使用するには必ず設定する。
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    // 引数には画面に表示する最大パーティクル数を設定する。
    if (Effkseer_Init(20000) == -1) { DxLib_End(); }

    // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
    // Effekseerを使用する場合は必ず設定する。
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    // DXライブラリのデバイスロストした時のコールバックを設定する。
    // ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();

    SetLightEnable(TRUE);


    // Zバッファを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetUseZBuffer3D(TRUE);

    // Zバッファへの書き込みを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetWriteZBuffer3D(TRUE);

    // 描画先画面を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);

    SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
    SetUseBackCulling(TRUE);		// バックカリングを行う

    

    SetUseSetDrawScreenSettingReset(false);

	now_scene_ = std::make_shared<Title>();

	now_scene_->Init();
	now_scene_name_ = now_scene_->GetName();

    FPS::GetInstance();
    Timer::GetInstance();
    Input::GetInstance().Awake(DX_INPUT_PAD1);
}

SceneManager::~SceneManager()
{

}

void SceneManager::Update()
{
    SceneName before_name = now_scene_->GetName();
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        bool init = FALSE;
        
        Debug::GetInstance().Recet();
        Input::GetInstance().Update();
        Debug::GetInstance().Change();
        //シーンが切り替わっている場合
        if (before_name != now_scene_name_)
        {
            now_scene_ = nullptr;
            switch (now_scene_name_)
            {
            case SceneName::kTitle:
                now_scene_ = std::make_shared<Title>();
                break;

            case SceneName::kGame:
                now_scene_ = std::make_shared<Game>();
                break;

            case SceneName::kResult:
                now_scene_ = std::make_shared<Result>();
                break;
            }
            
            now_scene_->Init();
            before_name = now_scene_->GetName();
            init = TRUE;
        }

        //initしたときちょっと1f遅れさせる
        FPS::GetInstance().Update();
        Timer::GetInstance().Update();

        if (!init)
        {
            now_scene_->Update(now_scene_name_);

            ClearDrawScreen();
        }
        
        
        //ここで描画処理
        now_scene_->Draw();

        //ここでデバック処理

        Timer::GetInstance().Debug();

        ScreenFlip();

        FPS::GetInstance().Wait();
        FPS::GetInstance().SetPrevTime();

        


    }

	

    Effkseer_End();
    DxLib_End();

    
}
