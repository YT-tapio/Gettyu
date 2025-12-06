#pragma once
#include"base_scene.h"
#include"vector_assistant.h"
#include"const_rad.h"
class BaseScene;

class Result : public BaseScene
{
private:

	const int kInitFadeInParamMax = 255;

	const float kNear	= 1.f;
	const float kFar	= 100.f;

	const float kFov = kOneRad * 75.f;

	const VECTOR kCameraPos = VGet(0.f, 0.f, -10.f);
	const VECTOR kTargetPos = VGet(0.f, 0.f, 10.f);

	const VECTOR kPos		= VGet(0.f, 0.f, 0.f);
	const VECTOR kScale		= VGet(0.01f, 0.01f, 0.01f);
	const VECTOR kRotation	= VectorAssistant::GetZeroVec();

	MATRIX mat_;

	bool is_fade_in_;
	float fade_in_param_;

	int model_;

	void FadeIn();

	//posやアニメーションの設定をする
	void Setting();

public:

	Result(int model);

	~Result() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;

};
