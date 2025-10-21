#pragma once
#include"base_scene.h"

class BaseScene;

class Title : public BaseScene
{
private:


public:

	Title();

	~Title() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;
};