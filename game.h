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

class Game : public BaseScene
{
private:

	std::shared_ptr<EffectManager>effect_player_;
	std::shared_ptr<Player>player_;
	std::shared_ptr<Brain>brain_;
	std::shared_ptr<Stage>stage_;
	std::shared_ptr<EnemyManager>enemy_manager_;
	std::shared_ptr<SkyDom> sky_dom_;
	std::shared_ptr<BaseSubScreen> concentration_line_;

	std::shared_ptr<ShadowMap> shadow_map_ = std::make_shared<ShadowMap>();

	std::shared_ptr<NormalSubScreen> screen_;

	//UIŒQ
	std::shared_ptr<WeaponUI> weapon_UI_;
	std::shared_ptr<SuperAttackUI> super_attack_UI_;
	std::shared_ptr<EnemyCountUI> enemy_count_UI_;

	void DrawShadowMap();

	void ScreenDraw();

public:

	Game();

	~Game() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;

	void End();
};
