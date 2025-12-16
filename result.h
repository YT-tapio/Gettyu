#pragma once
#include"base_scene.h"
#include"vector_assistant.h"
#include"const_rad.h"
#include"Animation.h"

class BaseScene;
class Button;
class ButtonSelecter;
class ObjectBase;
class NormalSubScreen;
class Font;
class SoundBase;

class Result : public BaseScene
{
private:

	const AnimationType kAnimType = AnimationType::kIdle;
	const int kInitFadeInParamMax = 255;

	const float kNear	= 1.f;
	const float kFar	= 1000.f;

	const float kFov = kOneRad * 75.f;

	const VECTOR kCameraPos		= VGet(0.f, 0.f, -10.f);
	const VECTOR kTargetPos		= VGet(0.f, 0.f, 10.f);

	const VECTOR kPos				= VGet(-10.f, -15.f, 10.f);
	const VECTOR kScale				= VectorAssistant::GetSame3DVec(0.01f);
	const VECTOR kRotation			= VectorAssistant::GetZeroVec();

	const int kFontSize				= 200;
	const int kFontThick			= 50;
	const int kFontColor			= GetColor(255, 255, 15);
	const int kFontThickColor	= GetColor(240, 44, 44);
	const VECTOR kClearTimerPos = VectorAssistant::Get2DVec(580.f, 550.f);
	std::shared_ptr<Font> tanuei_font_;			//たぬえいのフォント
	
	std::shared_ptr<SoundBase> bgm_;

	std::shared_ptr<NormalSubScreen> time_screen_;

	std::vector<std::shared_ptr<NormalSubScreen>> enemy_screens_;

	std::vector<std::shared_ptr<ObjectBase>> objects_;

	// 背景オブジェクト
	std::vector<std::shared_ptr<ObjectBase>> back_objects_;

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

	/// <summary>
	/// enemyを表示するscreenのupdate
	/// </summary>
	void UpdateDispEnemyScreen();

public:

	Result(int model);

	~Result() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;

};
