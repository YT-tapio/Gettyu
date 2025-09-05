#include<iostream>
#include<vector>
#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"animation.h"
#include"player.h"
#include"camera.h"
#include"effect_manager.h"
#include"FPS.h"
#include"mixamo_fram.h"
#include"base_object.h"
#include"still_object.h"
#include"brain.h"
#include"stage.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(1280, 832, 32);			//ウィンドウのサイズとカラーモードを決める
    ChangeWindowMode(TRUE);				//ウィンドウモードにする
    if (DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
    {
        return -1;        // エラーが起きたら直ちに終了
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

    SetMouseDispFlag(FALSE);

    int red = GetColor(255, 0, 0);
    int green = GetColor(0, 255, 0);
    int blue = GetColor(0, 0, 255);

    //全体のタイムスケール
    float time_scale = 1.0f;

    /*--キャラクターのダウンロード--*/

    int chara = MV1LoadModel("data/model/character/Dreyar_By_M.Aure.mv1");

    /*-----ダウンロードするアニメーション----*/


    AnimationData idle;
    AnimationData walk;
    AnimationData slow_run;
    AnimationData fast_run;
    AnimationData jumping_up;
    AnimationData jumping_down;
    AnimationData sword_slash_attack;

    
    std::shared_ptr<EffectManager>effect_player = 
       std::make_shared<EffectManager>("",1.0f,120);
   
   
    char idle_path[256]             = "data/animation/Idle.mv1";
    char walk_path[256]             = "data/animation/Walking.mv1";
    char slow_run_path[256]         = "data/animation/Slow_Run.mv1";
    char fast_run_path[256]         = "data/animation/Fast_Run.mv1";
    char jumping_up_path[256]       = "data/animation/Jumping_Up.mv1";
    char jumping_down_path[256]     = "data/animation/Jumping_Down.mv1";
    char sword_slash_path[256]      = "data/animation/SwordSlash.mv1";

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

    Load(sword_slash_attack, sword_slash_path,
        AnimationType::kSwordSlash, chara, 4.0f);

    //playerを生成
    std::shared_ptr<Player>player = 
        std::make_shared<Player>(VGet(0, 10, 100), chara, DX_INPUT_PAD1, 20, 1.5f, 5.0f);

    //brainを生成
    std::shared_ptr<Brain>brain = std::make_shared<Brain>(player->GetCenterPos());

    //カメラを生成
    std::shared_ptr<Camera>camera = std::make_shared<Camera>(brain->GetPositionFromTarget(player->GetCenterPos()),player->GetCenterPos(), 75.0f * DX_PI_F / 180.0f);

    //std::vector<std::shared_ptr<BaseObject>>objects;

    int model_data = MV1LoadModel("data/model/map/arena/map.mv1");

    std::shared_ptr<Stage>stage = std::make_shared<Stage>(model_data, VGet(0, 0, 0), 1.0f);
    //objects.push_back(std::make_shared<StillObject>(VGet(0, 0, 0), MV1LoadModel("data/model/map/block/block.mv1")));
    
    

    //オブジェクトを生成
    std::shared_ptr<BaseObject>object = 
        std::make_shared<StillObject>(VGet(50, 0, 10), -1,1.0f);

    /*---プレイヤーにアニメーションを追加---*/

    player->AddAnim(idle);
    player->AddAnim(walk);
    player->AddAnim(slow_run);
    player->AddAnim(fast_run);
    player->AddAnim(jumping_up);
    player->AddAnim(jumping_down);
    player->AddAnim(sword_slash_attack);

    MixamoBonePath bone;

    player->AttachWeapon(bone.RIGHT_HAND,
        MV1LoadModel("data/model/weapon/use_path/Bug_Net3.mv1"), 1.0f);




    //高精度タイマーでフレーム管理
   std::shared_ptr<FPS>fps = std::make_shared<FPS>();



    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        //現在の時間を取得
        fps->Update();
        //camera->GetPos();
        
        //更新処理
        player->SetDeltaTime(fps->GetDeltaTime());
        brain->SetDeltaTime(fps->GetDeltaTime());
        //test_effect1->SetDeltaTime(fps->GetDeltaTime());

        player->InputState();

        if (GetMouseInput() & MOUSE_INPUT_RIGHT)
        {
            player->SetIsTarget(TRUE);
        }
        else
        {
            player->SetIsTarget(FALSE);
        }
       

       
        
        
        player->Update(camera->GetPos(), brain->GetSideRad(),*stage);

        if (player->GetIsSwitchWeapon())
        {
            player->AttachWeapon(bone.RIGHT_HAND,
                MV1LoadModel("data/model/weapon/use_path/Bat.mv1"), 8.0f);
        }
        else
        {
            player->AttachWeapon(bone.RIGHT_HAND,
                MV1LoadModel("data/model/weapon/use_path/Bug_Net3.mv1"),5.0f);
        }

        if (!(player->GetIsTarget()))
        {
            //マウスでの操作
            brain->Update(camera->GetTargetPos(), camera->GetPos(),player);
        }
        else  //カメラは操作できなくとも位置は更新される
        {
            brain->SetRad(object->GetPos(), player->GetPos());
            brain->SetVelocity(player->GetPos(), camera->GetPos());
        }

        camera->Update(brain->GetVelocity(), brain->GetTargetVelocity());
        effect_player->Update();

        
        if (CheckHitKey(KEY_INPUT_1))
        {
            time_scale += 0.01;
        }

        if (CheckHitKey(KEY_INPUT_2))
        {
            time_scale -= 0.01f;
            if (time_scale < 0.0f)
            {
                time_scale = 0.0f;
            }
        }

        if (CheckHitKey(KEY_INPUT_R))
        {
            time_scale = 1.0f;
        }

        fps->SetTimeScale(time_scale);

        ClearDrawScreen();

        /*-----------------描画処理------------------*/

        /*----デルタタイム表示----*/
        //DrawFormatString(100, 100, GetColor(255, 255, 255), "%f", fps->GetDeltaTime());
        
        //fps->Draw();

        
        /*
        DrawString(0, 0, "x", red);
        DrawString(15, 0, "y", green);
        DrawString(30, 0, "z", blue);

        //中心をわかりやすくするため
        DrawLine3D(VGet(10, 0, 0), VGet(-10, 0, 0), red);
        DrawLine3D(VGet(0, 10, 0), VGet(0, -10, 0), green);
        DrawLine3D(VGet(0, 0, 10), VGet(0, 0, -10), blue);

        */
        

        //DrawLine3D(object->GetPos(), player->GetPos(),GetColor(100,240,50));

        //DrawLine3D(VGet(player->GetPos().x, 0.0f, player->GetPos().z),VGet(camera->GetPos().x, 0.0f, camera->GetPos().z), GetColor(123, 145, 9));
            

        //プレイヤーを中心とした座標軸を描画
        /*
        DrawLine3D(VGet((player->GetPos().x), 0, 0), VGet(-10, 0, 0), red);
        DrawLine3D(VGet(0, 10, 0), VGet(0, -10, 0), green);
        DrawLine3D(VGet(0, 0, 10), VGet(0, 0, -10), blue);
        */
        

        player->Draw();

        

        SetUseLighting(FALSE);

        stage->Draw();
        effect_player->Draw();

        //object->Draw();

        //DrawCapsule3D(VGet(0, 0, 0), VGet(10, 10, 10), 2, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);

        //brain->Draw();

        //fps->DrawTimeScale();

        

        SetUseLighting(TRUE);

        ScreenFlip();

        

        fps->Wait();

        fps->SetPrevTime();
        
    }

    Effkseer_End();

    DxLib_End();

    return 0;


}