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


class Game
{
private:

	std::shared_ptr<EffectManager>effect_player;
	std::shared_ptr<Player>player;
	std::shared_ptr<Brain>brain;
	std::shared_ptr<Camera>camera;
	std::shared_ptr<Stage>stage;
	std::shared_ptr<EnemyManager>enemy_manager;
	std::shared_ptr<SkyDom> sky_dom;
	std::shared_ptr<FPS>fps;
public:

	Game();

	~Game();

	void Awake();

	void Loop();

	void End();
};
