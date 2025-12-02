#pragma once
#include<iostream>
#include"base_scene.h"
#include"time.h"

class BaseScene;

class SceneManager
{
private:

	std::shared_ptr<BaseScene> now_scene_;
	SceneName now_scene_name_;


public:

	SceneManager();

	~SceneManager();

	void Update();

	void End();
};