#pragma once
#include<iostream>
#include<vector>
#include"base_scene.h"
#include"vector_assistant.h"

class BaseScene;
class ButtonSelecter;
class Button;
class Font;
class NormalSubScreen;

class Title : public BaseScene
{
private:

	const VECTOR kPos			= VGet(-5.f, -10.f, 15.f);
	const VECTOR kScale			= VectorAssistant::GetSame3DVec(0.01f);
	const VECTOR kRotation		= VectorAssistant::GetZeroVec();

	const char* kTanueiFontPath = "data/font/TanueiKakuPop_1_00/TanueiKakuPop.otf";
	const char* kTanueiFontName = "たぬえいカクポップタイ";

	const int kFontSize			= 800;
	const int kFontThick		= 40;

	MATRIX mat_;

	int button_num_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::shared_ptr<ButtonSelecter> selecter_;

	std::shared_ptr<Font> tanuei_font_;

	std::shared_ptr<NormalSubScreen> title_ui_screen_;

	bool start_;
	bool go_input_type_;
	bool game_end_;	

	/// <summary>
	/// カメラのセットやmodelのposのセットを行う
	/// </summary>
	void Setting();

public:

	Title(int model);

	~Title() override;

	void Init() override;

	void Update(SceneName& name) override;

	void Draw() override;
};