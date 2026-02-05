#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"screen.h"
#include"animation.h"
#include"player.h"
#include"camera.h"
#include"effect_manager.h"
#include"FPS.h"
#include"object_base.h"
#include"brain.h"
#include"stage.h"
#include"enemy_manager.h"
#include"situation.h"
#include"sky_dom.h"
#include"sub_screen.h"
#include"base_scene.h"
#include"shadow_map.h"
#include"weapon_UI.h"
#include"super_attack_UI.h"
#include"enemy_count_UI.h"

class BaseScene;
class Font;
class HitEffect;
class SoundBase;
class GameGoalUI;
class CountDownUI;
class InputInfoUI;

class Game : public BaseScene
{
private:

	const VECTOR kTimerPos			= VectorAssistant::Get2DVec(640.f, 30.f);
	const int kFontSize				= 100;
	const int kFontThickSize		= 50;
	const int kFontColor			= GetColor(255, 255, 15);

	const float kFadeInSpeed = 5.f;

	const float kFontColorGraphWidth	= 100.f;
	const float kFontColorGraphHeight	= 100.f;

	const char* kFontColorPath			= "data/font/color/UI_color.png";

	std::shared_ptr<EffectManager>effect_player_;
	std::shared_ptr<Player>player_;
	std::shared_ptr<Brain>brain_;
	std::shared_ptr<Stage>stage_;
	std::shared_ptr<EnemyManager>enemy_manager_;
	std::shared_ptr<SkyDom> sky_dom_;
	std::shared_ptr<BaseSubScreen> concentration_line_;

	std::shared_ptr<HitEffect> hit_effect_;

	std::shared_ptr<NormalSubScreen> screen_;
	std::shared_ptr<NormalSubScreen> font_color_screen_;
	std::shared_ptr<NormalSubScreen> timer_screen_;

	// サウンド
	std::shared_ptr<SoundBase> bgm_sound_;
	std::shared_ptr<SoundBase> clear_sound_;
	std::shared_ptr<SoundBase> clear_bomb_sound_;
	
	// UI群
	std::shared_ptr<WeaponUI> weapon_UI_;
	std::shared_ptr<SuperAttackUI> super_attack_UI_;
	std::shared_ptr<EnemyCountUI> enemy_count_UI_;
	std::shared_ptr<GameGoalUI> game_goal_UI_;
	std::shared_ptr<CountDownUI> count_down_UI_;
	std::shared_ptr<InputInfoUI> input_info_UI_;

	std::shared_ptr<Font> tanuei_font_;

	// タイマー
	std::shared_ptr<ConditionTimer>		stand_by_timer_;
	std::shared_ptr<ConditionTimer>		game_start_;
	std::shared_ptr<ConditionTimer>		clear_offset_timer_;
	std::shared_ptr<ConditionTimer>		clear_timer_;

	VECTOR center_pos_	= VGet(0.f, 0.f, 0.f);
	VECTOR norm_		= VGet(0.f, 1.f, 0.f);
	VECTOR x_norm = VGet(0.f, 0.f, 0.f);
	VECTOR y_norm = VGet(0.f, 0.f, 0.f);
	VECTOR z_norm = VGet(0.f, 0.f, 0.f);

	float x_rad_ = 90.f;
	float y_rad_ = 90.f;
	float z_rad_ = 90.f;

	int color_handle_;

	float offset_fade_param_;		// ゲーム終了のfadeoutやfadeinの


	void GameStart();

	void DrawShadowMap();

	void ScreenDraw();

	void TimeScreenDraw();

	void GameClear(SceneName& name);

	void FadeOut();

	void UpdateHitStop();

	void UpdateSound();

	void UpdateStandBy();

	bool IsCount();

public:

	Game(int model);

	~Game() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;

	void End();
};
