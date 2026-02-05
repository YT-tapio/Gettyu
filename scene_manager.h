#pragma once
#include<iostream>
#include"base_scene.h"
#include"time.h"

class BaseScene;
class LoadUI;

class SceneManager
{
private:

	// playerのモデルを先にダウンロードしておく
	const char* kPlayerModelPath = "data/model/character/Dreyar_By_M.Aure.mv1";
	int player_model_;

	std::shared_ptr<BaseScene> now_scene_;
	SceneName now_scene_name_;

	std::shared_ptr<LoadUI> load_ui_;

public:

	SceneManager();

	~SceneManager();

	void Update();

	void End();
};