#pragma once
#include"base_scene.h"
#include"vector_assistant.h"
#include"const_rad.h"
#include"Animation.h"

class BaseScene;
class Button;
class ButtonSelecter;

class Result : public BaseScene
{
private:

	const AnimationType kAnimType = AnimationType::kIdle;
	const int kInitFadeInParamMax = 255;

	const float kNear	= 1.f;
	const float kFar	= 100.f;

	const float kFov = kOneRad * 75.f;

	

	const VECTOR kCameraPos = VGet(0.f, 0.f, -10.f);
	const VECTOR kTargetPos = VGet(0.f, 0.f, 10.f);

	const VECTOR kPos		= VGet(-10.f, -15.f, 10.f);
	const VECTOR kScale		= VGet(0.01f, 0.01f, 0.01f);
	const VECTOR kRotation	= VectorAssistant::GetZeroVec();

	
	int button_num_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::shared_ptr<ButtonSelecter> selecter_;

	std::shared_ptr<Animation> animation_;

	MATRIX mat_;

	bool is_fade_in_;
	bool go_title_;
	
	float fade_in_param_;
	float time_;

	int enemy_model_;

	void FadeIn();

	//posやアニメーションの設定をする
	void Setting();

	void AddAnim();

public:

	Result(int model);

	~Result() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;

};
