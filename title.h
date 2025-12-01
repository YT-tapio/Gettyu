#pragma once
#include"base_scene.h"

class BaseScene;
class Button;

class Title : public BaseScene
{
private:


	std::shared_ptr<Button> button_;


public:

	Title();

	~Title() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;
};