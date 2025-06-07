#include<iostream>
#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"animation.h"
#include"player.h"
#include"camera.h"
#include"effect_manager.h"
#include"FPS.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(1280, 832, 32);			//ウィンドウのサイズとカラーモードを決める
    ChangeWindowMode(TRUE);				//ウィンドウモードにする
    if (DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
    {
        return -1;        // エラーが起きたら直ちに終了
    }

    // 描画先画面を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);

    SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
    SetUseBackCulling(TRUE);		// バックカリングを行う

    SetMouseDispFlag(FALSE);

    int red = GetColor(255, 0, 0);
    int green = GetColor(0, 255, 0);
    int blue = GetColor(0, 0, 255);

    /*--キャラクターのダウンロード--*/

    int chara = MV1LoadModel("data/model/Dreyar_By_M.Aure.mv1");

    /*-----ダウンロードするアニメーション----*/


    AnimationData idle;
    AnimationData walk;
    AnimationData slow_run;
    AnimationData fast_run;
    AnimationData jumping_up;
    AnimationData jumping_down;

   std::shared_ptr<EffectManager>effect_manager = std::make_shared<EffectManager>();

    char idle_path[256] = "data/animation/Idle.mv1";
    char walk_path[256] = "data/animation/Walking.mv1";
    char slow_run_path[256] = "data/animation/Slow_Run.mv1";
    char fast_run_path[256] = "data/animation/Fast_Run.mv1";
    char jumping_up_path[256] = "data/animation/Jumping_Up.mv1";
    char jumping_down_path[256] = "data/animation/Jumping_Down.mv1";

    Load(idle, idle_path,
        AnimationType::kIdle, chara,3.0f);

    Load(walk, walk_path,
        AnimationType::kWalk, chara, 3.0f);

    Load(slow_run, slow_run_path,
        AnimationType::kSlowRun, chara, 3.0f);

    Load(fast_run, fast_run_path,
        AnimationType::kFastRun, chara, 3.0f);

    Load(jumping_up, jumping_up_path, 
        AnimationType::kJumpUp, chara, 2.0f);

    Load(jumping_down, jumping_down_path,
        AnimationType::kJumpDown, chara, 2.0f);

    //カメラを生成
    std::shared_ptr<Camera>camera = std::make_shared<Camera>();

    //playerを生成
    std::shared_ptr<Player>player = 
        std::make_shared<Player>(VGet(0,0,0),chara,DX_INPUT_PAD1);

    /*---プレイヤーにアニメーションを追加---*/

    player->AddAnim(idle);
    player->AddAnim(walk);
    player->AddAnim(slow_run);
    player->AddAnim(fast_run);
    player->AddAnim(jumping_up);
    player->AddAnim(jumping_down);

    //高精度タイマーでフレーム管理
   std::shared_ptr<FPS>fps = std::make_shared<FPS>();

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        //現在の時間を取得
        fps->Update();

        
        //更新処理
        player->SetDeltaTime(fps->GetDeltaTime());
        player->Update(camera->GetPos(),camera->GetSideRad());
        camera->Update(player->GetCenterPos());
        effect_manager->Update(player->GetPos());
       
        

        ClearDrawScreen();

        /*-----------------描画処理------------------*/

        /*----デルタタイム表示----*/
        DrawFormatString(100, 100, GetColor(255, 255, 255), "%f", fps->GetDeltaTime());
        
        fps->Draw();

        DrawString(0, 0, "x", red);
        DrawString(15, 0, "y", green);
        DrawString(30, 0, "z", blue);

        //中心をわかりやすくするため
        DrawLine3D(VGet(10, 0, 0), VGet(-10, 0, 0), red);
        DrawLine3D(VGet(0, 10, 0), VGet(0, -10, 0), green);
        DrawLine3D(VGet(0, 0, 10), VGet(0, 0, -10), blue);

        player->Draw();
        effect_manager->Draw();

        ScreenFlip();

        fps->Wait();

        fps->SetPrevTime();
        
    }

    Effkseer_End();

    DxLib_End();

    return 0;


}