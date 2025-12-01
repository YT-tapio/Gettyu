#pragma once
#include<iostream>
#include<vector>
#include"base_scene.h"

class BaseScene;
class Button;

class Title : public BaseScene
{
private:

	int button_num_;
	std::vector<std::shared_ptr<Button>> buttons_;


public:

	Title();

	~Title() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;
};