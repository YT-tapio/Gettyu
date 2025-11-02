#include"game.h"
#include"input.h"
#include"debug.h"
#include"keyconfig.h"
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

//


void Game::Init()
{
    int red = GetColor(255, 0, 0);
    int green = GetColor(0, 255, 0);
    int blue = GetColor(0, 0, 255);

    
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
    AnimationData super_attack_first;

    effect_player_ =
        std::make_shared<EffectManager>("", 1.0f, 120);



    char idle_path[256] = "data/animation/Idle.mv1";
    char walk_path[256] = "data/animation/Walking.mv1";
    char slow_run_path[256] = "data/animation/Slow_Run.mv1";
    char fast_run_path[256] = "data/animation/Fast_Run.mv1";
    char jumping_up_path[256] = "data/animation/Jumping_Up.mv1";
    char jumping_down_path[256] = "data/animation/Jumping_Down.mv1";
    char sword_slash_path[256] = "data/animation/SwordSlash.mv1";
    char super_attack_path[256] = "data/animation/Standing_2H_Cast_Spell_01.mv1";

    //アニメーションのロード

    Load(idle, idle_path,
        AnimationType::kIdle,chara, 0,3.0f);

    Load(walk, walk_path,
        AnimationType::kWalk, chara, 0, 3.0f);

    Load(slow_run, slow_run_path,
        AnimationType::kSlowRun, chara, 0, 3.0f);

    Load(fast_run, fast_run_path,
        AnimationType::kFastRun, chara, 0, 3.0f);

    Load(jumping_up, jumping_up_path,
        AnimationType::kJumpUp, chara, 0, 2.0f);

    Load(jumping_down, jumping_down_path,
        AnimationType::kJumpDown, chara, 0, 2.0f);

    Load(sword_slash_attack, sword_slash_path,
        AnimationType::kSwordSlash, chara, 0, 4.0f);

    Load(super_attack_first, super_attack_path,
        AnimationType::kSuperAttackFirst, chara, 0, 3.0f);

    //playerを生成
    player_ =
        std::make_shared<Player>(VGet(0, 10, 100), chara, DX_INPUT_PAD1, 20, 1.5f, 5.0f);

    brain_ = std::make_shared<Brain>(player_->GetCenterPos());

    camera_ = std::make_shared<Camera>(brain_->GetPositionFromTarget(player_->GetCenterPos()),
        player_->GetCenterPos(), (DX_PI_F / 180.0f) * 75.0f);

    int model_data = MV1LoadModel("data/model/map/arena/map.mv1");

    stage_ = std::make_shared<Stage>(model_data, VGet(0, 0, 0), 1.0f);

    brain_->Init(camera_->GetPos(), player_->GetCenterPos());
    camera_->Init(brain_->GetVelocity());

    /*---プレイヤーにアニメーションを追加---*/

    player_->AddAnim(idle);
    player_->AddAnim(walk);
    player_->AddAnim(slow_run);
    player_->AddAnim(fast_run);
    player_->AddAnim(jumping_up);
    player_->AddAnim(jumping_down);
    player_->AddAnim(sword_slash_attack);
    player_->AddAnim(super_attack_first);

    enemy_manager_ =
        std::make_shared<EnemyManager>();

    enemy_manager_->Init();

    sky_dom_                  = std::make_shared<SkyDom>("data/skydome/Dome_SS601.mv1", camera_->GetPos());
    concentration_line_     = std::make_shared<ConcentrationLine>(kGameWidth, kGameHeight, TRUE);

    weapon_UI_               = std::make_shared<WeaponUI>();
    super_attack_UI_        = std::make_shared<SuperAttackUI>();
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
    

    player_->InputState();

    

    enemy_manager_->Update(player_);
    player_->Update(camera_->GetPos(), brain_->GetSideRad(), *stage_);


    //マウスでの操作
    brain_->Update(camera_->GetTargetPos(), camera_->GetPos(), player_);


    //UIのアップデート
    weapon_UI_->Update();
    super_attack_UI_->Update();


    camera_->Update(brain_->GetVelocity(), brain_->GetTargetVelocity());
    effect_player_->Update();

    

    sky_dom_->SetPos(player_->GetVelocity());
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

}

void Game::Draw()
{
   
    DrawShadowMap();


    shadow_map_->UseShadowMap();

    

    player_->Draw();
    enemy_manager_->Draw();

    sky_dom_->Draw();

    SetUseLighting(FALSE);
    stage_->Draw();
    SetUseLighting(TRUE);
    

    shadow_map_->UnuseShadowMap();

    

    camera_->Draw();
    effect_player_->Draw();
    concentration_line_->Draw();

    weapon_UI_->Draw();
    super_attack_UI_->Draw();

    if (Debug::GetInstance().GetDisp())
    {
        player_->Debug();
        Input::GetInstance().Debug();
        enemy_manager_->Debug();
    }

    

    DrawFormatString((kGameWidth - 300), (kGameHeight - 30), GetColor(0, 0, 0), "TAB / BACK Button : result");

    SetUseLighting(TRUE);
}


void Game::End()
{
    Effkseer_End();
    DxLib_End();
}