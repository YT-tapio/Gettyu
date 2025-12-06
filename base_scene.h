#pragma once
#include"DxLib.h"

enum class SceneName
{
	kTitle,
	kGame,
	kResult,
	kEnd
};


class BaseScene
{
private:

	
	SceneName name_;

protected:

	int player_model_;

public:


	BaseScene(SceneName name,int model);

	virtual ~BaseScene() = 0;

	virtual void Init() = 0;

	virtual void Update(SceneName& name) = 0;

	virtual void Draw() = 0;

	const SceneName GetName() { return name_; }

};
