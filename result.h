#pragma once
#include"base_scene.h"

class BaseScene;

class Result : public BaseScene
{
private:



public:

	Result();

	~Result() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;

};
