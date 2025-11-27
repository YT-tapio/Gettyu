#include"game.h"
#include"input.h"
#include"debug.h"
#include"keyconfig.h"
#include"gauss.h"
#include"mask.h"
#include"normal_sub_screen.h"
Game::Game()
    :BaseScene(SceneName::kGame)
{
    
}

Game::~Game()
{
    
}


void Game::DrawShadowMap()
{
    shadow_map_->SetupDrawShadowMap();

    player_->Draw();
    enemy_manager_->Draw();


    stage_->Draw();

    //SetUseLighting(TRUE);

    shadow_map_->EndDrawShadowMap();
}


void Game::ScreenDraw()
{
    screen_->Up();

    player_->Draw();
    enemy_manager_->Draw();

    sky_dom_->Draw();

    SetUseLighting(FALSE);
    stage_->Draw();
    SetUseLighting(TRUE);


    Camera::GetInstance().Draw();
    effect_player_->Draw();
    concentration_line_->Draw();

    

    if (Debug::GetInstance().GetDisp())
    {
        stage_->Debug();
        player_->Debug();
        Input::GetInstance().Debug();
        enemy_manager_->Debug();
    }

    screen_->Down();
    

}

//


void Game::Init()
{
    int red = GetColor(255, 0, 0);
    int green = GetColor(0, 255, 0);
    int blue = GetColor(0, 0, 255);

    
    effect_player_ =
        std::make_shared<EffectManager>("", 1.0f, 120);

    //playerを生成
    player_ =
        std::make_shared<Player>(VGet(0, 10, 100), DX_INPUT_PAD1, 20, 1.5f, 5.0f);

    brain_ = std::make_shared<Brain>(player_->GetCenterPos());

    Camera::GetInstance().Awake(brain_->GetPositionFromTarget(player_->GetCenterPos()),
        player_->GetCenterPos(), (DX_PI_F / 180.0f) * 75.0f);

    int model_data = MV1LoadModel("data/model/map/arena/map.mv1");

    stage_ = std::make_shared<Stage>(model_data, VGet(0, 0, 0), 1.0f);

    brain_->Init(Camera::GetInstance().GetPos(), player_->GetCenterPos());
    Camera::GetInstance().Init(brain_->GetVelocity());

    /*---プレイヤーにアニメーションを追加---*/


    enemy_manager_ =
        std::make_shared<EnemyManager>();

    enemy_manager_->Init();

    sky_dom_                    = std::make_shared<SkyDom>("data/skydome/Dome_SS601.mv1", Camera::GetInstance().GetPos());
    concentration_line_         = std::make_shared<ConcentrationLine>(kGameWidth, kGameHeight, TRUE);

    weapon_UI_                  = std::make_shared<WeaponUI>();
    super_attack_UI_            = std::make_shared<SuperAttackUI>();

    enemy_count_UI_ = std::make_shared<EnemyCountUI>(&enemy_manager_->not_get_count_);

    screen_ = std::make_shared<NormalSubScreen>(VGet((kGameWidth * 0.5f), (kGameHeight * 0.5f), 0.f), kGameWidth, kGameHeight,
        kGameWidth, kGameHeight, FALSE, AlphaColorType::kBlack, 0.f, FALSE);

    screen_->SetIsDisp(TRUE);
}

void Game::Update(SceneName& name)
{
    //全体のタイムスケール
    static float time_scale = 1.0f;
    
    //現在の時間を取得
    
    //camera->GetPos();

    //デバッグ用
    if (Input::GetInstance().CheckInputKey(KeyConfig::kGameToResultKey) == InputState::kPush ||
        Input::GetInstance().CheckInputPadButton(PadConfig::kGameToResultButton) == InputState::kPush)
    {
        name = SceneName::kResult;
    }

    //更新処理

    //デルタタイムのアップデートはゲット時はplayerとenemyのだけ0にする

    player_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());
    brain_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());
    enemy_manager_->SetDeltaTime(FPS::GetInstance().GetDeltaTime());
    //test_effect1->SetDeltaTime(fps->GetDeltaTime());

    concentration_line_->Update();
    concentration_line_->SetIsDisp(player_->GetIsVacuum());
    

    

    enemy_manager_->Update(player_);
    player_->Update(*stage_);


    //マウスでの操作
    brain_->Update(Camera::GetInstance().GetTargetPos(), Camera::GetInstance().GetPos(), player_);

    //UIのアップデート
    weapon_UI_->Update();
    super_attack_UI_->Update();
    enemy_count_UI_->Update();
   

    Camera::GetInstance().Update(brain_->GetVelocity(), brain_->GetTargetVelocity());
    effect_player_->Update();
    sky_dom_->SetPos(player_->GetVelocity());

    //makscreenの中で描画する
    ScreenDraw();

    static int param = 100;
    static int pixel = 8;
    
    
    if (Input::GetInstance().CheckInputKey(KEY_INPUT_UP) == InputState::kOn)
    {
        Gauss::GetInstance().Update(screen_->GetHandle(), pixel, param);
    }
    

    //printfDx("Gauss：%d\n", pixel);

    SetUseLighting(TRUE);
    

    
    if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        time_scale += 0.01;
    }

    if (CheckHitKey(KEY_INPUT_LEFT))
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


    

    //ここら辺で終わりのやつを作りたいです

    if (enemy_manager_->CheckIsEnemy())
    {
        //ここで終了
        name = SceneName::kResult;
    }

    

    FPS::GetInstance().SetTimeScale(time_scale);

    // name = SceneName::kResult;
}

void Game::Draw()
{
    

    screen_->Draw();
    
    weapon_UI_->Draw();
    super_attack_UI_->Draw();
    enemy_count_UI_->Draw();

    DrawFormatString((kGameWidth - 300), (kGameHeight - 30), GetColor(0, 0, 0), "TAB / BACK Button : result");
}


void Game::End()
{
    Effkseer_End();
    DxLib_End();
}