#include<iostream>
#include"DxLib.h"
#include"player.h"
#include"camera.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(1280, 832, 32);			//ウィンドウのサイズとカラーモードを決める
    ChangeWindowMode(FALSE);				//ウィンドウモードにする
    if (DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
    {
        return -1;        // エラーが起きたら直ちに終了
    }

    // 描画先画面を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);

    SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
    SetUseBackCulling(TRUE);		// バックカリングを行う

    int red = GetColor(255, 0, 0);
    int green = GetColor(0, 255, 0);
    int blue = GetColor(0, 0, 255);

    //カメラを生成
    std::shared_ptr<Camera>camera = std::make_shared<Camera>();

    //playerを生成
    std::shared_ptr<Player>player = 
        std::make_shared<Player>(VGet(0,0,0),MV1LoadModel("data/model/Y_Bot.mv1"));

    //高精度タイマーでフレーム管理
    LONGLONG prevTime = GetNowHiPerformanceCount();

   

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        //現在の時間を取得
        LONGLONG nowTime = GetNowHiPerformanceCount();

       
        // deltaTime計測
        float delta_time;
        // nowCount = GetNowCount();
        delta_time = (nowTime - prevTime) / 100000.0f;

        //更新処理
        player->Update();
        camera->Update(player->GetPos());
       
        ClearDrawScreen();

        /*-----------------描画処理------------------*/

        DrawString(0, 0, "x", red);
        DrawString(15, 0, "y", green);
        DrawString(30, 0, "z", blue);

        //中心をわかりやすくするため
        DrawLine3D(VGet(10, 0, 0), VGet(-10, 0, 0), red);
        DrawLine3D(VGet(0, 10, 0), VGet(0, -10, 0), green);
        DrawLine3D(VGet(0, 0, 10), VGet(0, 0, -10), blue);

        player->Draw();

        ScreenFlip();
    }

    DxLib_End();

    return 0;


}