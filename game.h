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
#include"base_object.h"
#include"still_object.h"
#include"brain.h"
#include"stage.h"
#include"enemy_manager.h"
#include"situation.h"
#include"sky_dom.h"
#include"sub_screen.h"
#include"base_scene.h"

class BaseScene;

class Game : public BaseScene
{
private:

	std::shared_ptr<EffectManager>effect_player;
	std::shared_ptr<Player>player;
	std::shared_ptr<Brain>brain;
	std::shared_ptr<Camera>camera;
	std::shared_ptr<Stage>stage;
	std::shared_ptr<EnemyManager>enemy_manager;
	std::shared_ptr<SkyDom> sky_dom;
	std::shared_ptr<FPS> fps;
	std::shared_ptr<BaseScreen> screen_;

public:

	Game();

	~Game() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;

	void End();
};
