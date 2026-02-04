#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include"base_scene.h"
#include"vector_assistant.h"
#include"const_rad.h"

class BaseScene;
class ButtonSelecter;
class Button;
class Font;
class NormalSubScreen;
class Animation;
class ConditionTimer;
class SoundBase;
class ObjectBase;


class Title : public BaseScene
{
private:

	const VECTOR kPos			= VGet(-20.f, -10.f, 35.f);
	const VECTOR kScale			= VectorAssistant::GetSame3DVec(0.01f);
	const VECTOR kRotation		= VGet(0.f, -kOneRad * 40.f, 0.f);

	const VECTOR kEnemyPos			= VGet(-5.f, -10.f, 25.f);
	const VECTOR kEnemyScale		= VectorAssistant::GetSame3DVec(0.07f);
	const VECTOR kEnemyRotation	= VGet(0.f, -kOneRad * 40.f, 0.f);

	const VECTOR kTitleUiPos		= VectorAssistant::Get2DVec(900.f, 350.f);
	
	const VECTOR kInitTitlePos = VectorAssistant::Get2DVec(80.f, 20.f);

	const VECTOR kGameStartButtonPos	= VectorAssistant::Get2DVec(900.f, 450.f);
	const VECTOR kGoTutorialButtonPos	= VectorAssistant::Get2DVec(900.f, 565.f);
	const VECTOR kGameEndButtonPos		= VectorAssistant::Get2DVec(900.f, 680.f);
	const float kButtonWidth						= 300.f;
	const float kButtonHeight					= 100.f;


	const int kFontSize		= 300;
	const int kFontThick	= 4;
	
	const int kTitleUiWidth		= 600;
	const int kTitleUiHeight		= 500;

	std::shared_ptr<Animation> animation_;
	std::shared_ptr<Animation> enemy_animation_;

	std::vector<std::shared_ptr<ObjectBase>> objects_;

	MATRIX mat_;
	MATRIX enemy_mat_;

	int button_num_;
	int before_button_num_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::shared_ptr<ButtonSelecter> selecter_;

	std::shared_ptr<Font> tanuei_font_;
	std::shared_ptr<NormalSubScreen> title_ui_screen_;

	std::shared_ptr<SoundBase> select_sound_;

	//画面遷移のtimer
	std::shared_ptr<ConditionTimer> transition_timer_;
	

	std::shared_ptr<SoundBase> bgm_;

	int enemy_model_;

	float title_ui_rad_;
	float fade_in_param_;

	bool start_;
	bool go_tutorial_;
	bool game_end_;	


	void AnimationSetting();

	/// <summary>
	/// カメラのセットやmodelのposのセットを行う
	/// </summary>
	void Setting();

	void FadeOut();

	

public:

	Title(int model);

	~Title() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;
};