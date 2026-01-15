#include"game.h"
#include"input.h"
#include"debug.h"
#include"keyconfig.h"
#include"gauss.h"
#include"mask.h"
#include"normal_sub_screen.h"
#include"Draw2D.h"
#include"offset_assistant.h"
#include"font.h"
#include"hit_stop_timer.h"
#include"hit_effect.h"
#include"enemy_get_num.h"
#include"sound.h"
#include"2D_sound.h"
#include"game_goal_ui.h"
#include"count_down_UI.h"

Game::Game(int model)
    :BaseScene(SceneName::kGame,model)
{
    
}

Game::~Game()
{
    DeleteGraph(color_handle_);
}

void Game::GameStart()
{
    float speed = kFadeInSpeed * FPS::GetInstance().GetDeltaTime();
    OffsetAssistant::Smallf(offset_fade_param_, 0, speed);
}

void Game::DrawShadowMap()
{
    player_->Draw();
    enemy_manager_->Draw();

    stage_->Draw();
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

void Game::TimeScreenDraw()
{
    font_color_screen_->Up();
    Draw2D::ExtendGraph(VectorAssistant::Get2DVec((kFontColorGraphWidth * 0.5f), (kFontColorGraphHeight * 0.5f)), kFontColorGraphWidth,kFontColorGraphHeight,color_handle_, TRUE);
    font_color_screen_->Down();

    timer_screen_->Up();
    Draw2D::Circle(VectorAssistant::Get2DVec((kFontColorGraphWidth * 0.5f), (kFontColorGraphHeight * 0.5f)), kFontColorGraphWidth * 0.4f, Color::kWhite, TRUE);
    timer_screen_->Down();

}

void Game::GameClear(SceneName& name)
{
    if (enemy_manager_->CheckIsEnemy()) { return; }

    //ここら辺で終わりのやつを作りたいです
    if (!clear_offset_timer_->GetIsEnd())
    {
        Situation::GetInstance().SetSituationName(SituationName::kClearOffset);
        clear_offset_timer_->Update();
    }
    else
    {
        //offset_timerのカウントが終わっているなら
        Situation::GetInstance().SetSituationName(SituationName::kClear);
        clear_timer_->Update();

        float timer_count = clear_timer_->GetNowTimer();

        if (timer_count >= 2.f)
        {
            clear_sound_->Stop();
        }

        if (timer_count >= 2.4f)
        {
            clear_bomb_sound_->Update();
        }
        

        if (clear_timer_->GetIsEnd())
        {
            //ここで終了
            name = SceneName::kResult;
        }

    }

    FadeOut();

}

void Game::FadeOut()
{
    const float kFadeOutSpeed         = 15.f;
    const float kFadeOutTime          = 3.2f;
    const int kParamMax                 = 255;

    if (clear_timer_->GetNowTimer() >= kFadeOutTime)
    {
        offset_fade_param_ += (kFadeOutSpeed * FPS::GetInstance().GetDeltaTime());

        offset_fade_param_ = (offset_fade_param_ > kParamMax) ? kParamMax : offset_fade_param_;
    }



}

bool Game::IsCount()
{
    auto name = Situation::GetInstance().GetSituationName();

    if (name == SituationName::kGet)                    { return FALSE; }
    if (name == SituationName::kPerformance)       { return FALSE; }
    if (name == SituationName::kClearOffset)         { return FALSE; }
    if (name == SituationName::kClear)                  { return FALSE; }

    return TRUE;
}

void Game::UpdateHitStop()
{
    HitStopTimer::GetInstance().TimerUpdate();  //
}

void Game::UpdateSound()
{
    // リスナーの設置
    Camera::GetInstance().SetListener();

    if (Situation::GetInstance().GetSituationName() >= SituationName::kClearOffset)
    {
        clear_sound_->Update();
        bgm_sound_->Stop();
    }

    bgm_sound_->Update();
}

void Game::UpdateStandBy()
{

    // ここでplayerに何かを伝えたりカウントダウンしてあげたりします
    // playerを動かせないようにします
    Situation::GetInstance().SetSituationName(SituationName::kStandBy);
    stand_by_timer_->Update();
    ClearTime::GetInstance().Stop();
    
    if (stand_by_timer_->GetIsEnd())
    {
        ClearTime::GetInstance().Start();
        Situation::GetInstance().SetSituationName(SituationName::kNothing);
        return;
    }

}

//


void Game::Init()
{
    Situation::GetInstance().Init();

    int mouse_init_pos_x = kGameWidth * 0.5f;
    int mouse_init_pos_y = kGameHeight * 0.5f;

    
    SetMouseDispFlag(FALSE);

    int red = GetColor(255, 0, 0);
    int green = GetColor(0, 255, 0);
    int blue = GetColor(0, 0, 255);

    
    effect_player_ = std::make_shared<EffectManager>("", 1.0f, 120);

    color_handle_ = LoadGraph(kFontColorPath);


    //playerを生成
    player_ = std::make_shared<Player>(VGet(0, 10, 100), player_model_,DX_INPUT_PAD1, 20, 2.0f, 10.0f);

    brain_ = std::make_shared<Brain>(player_->GetCenterPos());

    EnemyGetNum::GetInstance().Reset();

    Camera::GetInstance().Awake(brain_->GetPositionFromTarget(player_->GetCenterPos()),
        player_->GetCenterPos(), (DX_PI_F / 180.0f) * 75.0f);

    const char* kStagePath = "data/model/map/arena/map.mv1";

    stage_ = std::make_shared<Stage>(kStagePath, VGet(0, 0, 0), 1.0f);

    brain_->Init(Camera::GetInstance().GetPos(), player_->GetCenterPos());
    Camera::GetInstance().Init(brain_->GetVelocity());

    enemy_manager_ = std::make_shared<EnemyManager>(stage_);

    enemy_manager_->Init();

    sky_dom_                    = std::make_shared<SkyDom>("data/skydome/Dome_SS601.mv1", Camera::GetInstance().GetPos());  // スカイドーム
    concentration_line_         = std::make_shared<ConcentrationLine>(kGameWidth, kGameHeight, TRUE);       // 集中線

    hit_effect_                 = std::make_shared<HitEffect>();            // 敵に当たった時のeffect

    /*--Sound--*/
    const char* kBgmSoundPath                 = "data/sound/game/bgm/Ska_01.mp3";
    const char* kClearBgmSoundPath          = "data/sound/game/bgm/game_clear.mp3";
    const char* kClearBombSoundPath         = "data/sound/game/se/clear_bomb.mp3";

    bgm_sound_              = std::make_shared<Sound2D>(kBgmSoundPath, DX_PLAYTYPE_LOOP,100,TRUE);
    clear_sound_             = std::make_shared<Sound2D>(kClearBgmSoundPath, DX_PLAYTYPE_BACK, 100,FALSE);
    clear_bomb_sound_   = std::make_shared<Sound2D>(kClearBombSoundPath, DX_PLAYTYPE_BACK, 200, FALSE);
    /*--UI--*/

    weapon_UI_                      = std::make_shared<WeaponUI>();
    super_attack_UI_            = std::make_shared<SuperAttackUI>();
    enemy_count_UI_            = std::make_shared<EnemyCountUI>(&enemy_manager_->not_get_count_);
    game_goal_UI_                = std::make_shared<GameGoalUI>(&enemy_manager_->not_get_count_);
    count_down_UI_              = std::make_shared<CountDownUI>();

    tanuei_font_                = std::make_shared<Font>(kTanueiFontPath, kTanueiFontName, kFontSize, kFontThickSize, DX_FONTTYPE_EDGE);

    screen_                     = std::make_shared<NormalSubScreen>(VGet((kGameWidth * 0.5f), (kGameHeight * 0.5f), 0.f), kGameWidth, kGameHeight, kGameWidth, kGameHeight, FALSE, AlphaColorType::kBlack, 0.f, FALSE);
    font_color_screen_          = std::make_shared<NormalSubScreen>(VectorAssistant::Get2DVec(200.f, 200.f), kFontColorGraphWidth, kFontColorGraphHeight, kFontColorGraphWidth, kFontColorGraphHeight, TRUE, AlphaColorType::kBlack, 10, TRUE);
    timer_screen_               = std::make_shared<NormalSubScreen>(VectorAssistant::Get2DVec(200.f, 200.f), kFontColorGraphWidth, kFontColorGraphHeight, kFontColorGraphWidth - 10, kFontColorGraphHeight - 10, TRUE, AlphaColorType::kBlack, 10, TRUE);

    screen_->SetIsDisp(TRUE);
    font_color_screen_->SetIsDisp(TRUE);
    timer_screen_->SetIsDisp(TRUE);
    SetMousePoint(mouse_init_pos_x, mouse_init_pos_y);

    const float kStandByTime        = 10.f;
    const float kGameStartTime      = 5.f;
    const float kClearTime          = 8.f;
    const float kClearOffsetTime    = 1.f;

    stand_by_timer_     = std::make_shared<ConditionTimer>(kStandByTime);
    game_start_         = std::make_shared<ConditionTimer>(kGameStartTime);
    clear_timer_        = std::make_shared<ConditionTimer>(kClearTime);
    clear_offset_timer_ = std::make_shared<ConditionTimer>(kClearOffsetTime);
   
    offset_fade_param_  = 255.f;

    ClearTime::GetInstance().Reset();
}

void Game::Update(SceneName& name)
{

    if (HitStopTimer::GetInstance().CheckHitStop()) { UpdateHitStop(); }

    //name = SceneName::kResult;
    //全体のタイムスケール
    static float time_scale = 1.0f;



    if (!stand_by_timer_->GetIsEnd())
    {
        Input::GetInstance().NoActive();
        UpdateStandBy();
    }
    else
    {
        Input::GetInstance().Active();
    }

    GameStart();

    if (Situation::GetInstance().GetSituationName() == SituationName::kClearOffset) { ClearTime::GetInstance().Stop(); }
    ClearTime::GetInstance().Update();

    

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
    hit_effect_->SetDeltaTime();

    
    UpdateSound();

    concentration_line_->Update();
    concentration_line_->SetIsDisp(player_->GetIsVacuum());
    

    enemy_manager_->Update(player_);
    player_->Update(*stage_,brain_->GetSideRad());

    
    //マウスでの操作
    brain_->Update(Camera::GetInstance().GetTargetPos(), Camera::GetInstance().GetPos(), player_);

    //UIのアップデート
    weapon_UI_->Update();
    super_attack_UI_->Update();
    enemy_count_UI_->Update();
    game_goal_UI_->Update();
    count_down_UI_->Update(stand_by_timer_->GetNowTimer());

    hit_effect_->Update();

    Camera::GetInstance().Update(brain_->GetVelocity(), brain_->GetTargetVelocity());
    effect_player_->Update();
    sky_dom_->SetPos(player_->GetVelocity());
    TimeScreenDraw();
    //makscreenの中で描画する
    ScreenDraw();

    static int param = 100;
    static int pixel = 8;
    
    if (Input::GetInstance().CheckInputKey(KEY_INPUT_UP) == InputState::kOn)
    {
        Gauss::GetInstance().Update(screen_->GetHandle(), pixel, param);
    }
    
    SetUseLighting(TRUE);

    GameClear(name);

    FPS::GetInstance().SetTimeScale(time_scale);

    // name = SceneName::kResult;
}

void Game::Draw()
{
    screen_->Draw();
    
    weapon_UI_->Draw();
    super_attack_UI_->Draw();
    enemy_count_UI_->Draw();
    game_goal_UI_->Draw();
    count_down_UI_->Draw();

    float clear_time = ClearTime::GetInstance().GetClearTime();
    int timer_width = GetDrawFormatStringWidthToHandle(tanuei_font_->GetHandle(), "%.1f", clear_time);
    DrawFormatStringToHandle(static_cast<int>(kTimerPos.x - float(timer_width) * 0.5f), static_cast<int>(kTimerPos.y), kFontColor, tanuei_font_->GetHandle(), "%.1f", clear_time);
    Draw2D::WhiteBoxBlend(static_cast<int>(offset_fade_param_));
    //DrawFormatString((kGameWidth - 300), (kGameHeight - 30), GetColor(0, 0, 0), "TAB / BACK Button : result");
}


void Game::End()
{

}