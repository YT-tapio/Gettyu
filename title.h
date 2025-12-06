#pragma once
#include<iostream>
#include<vector>
#include"base_scene.h"

class BaseScene;
class ButtonSelecter;
class Button;

class Title : public BaseScene
{
private:

	

	int button_num_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::shared_ptr<ButtonSelecter> selecter_;

	bool start_;
	bool go_input_type_;
	bool game_end_;

	
	

public:

	Title(int model);

	~Title() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;
};