#pragma once

#include"vector_assistant.h"

class NormalSubScreen;
class Font;

class ResultScoreUI
{
private:

	const VECTOR kInitPos = VectorAssistant::Get2DVec(800.f, 470.f);

	const int kFontColor = GetColor(255, 215, 0);
	const int kFontSize = 100;

	VECTOR pos_;

	std::string sentence_;
	std::string down_sentence_;
	std::shared_ptr<NormalSubScreen> screen_;
	std::shared_ptr<Font> font_;
	
	float rad_;

	int screen_width_;
	int screen_height_;

	void DecideScore(const float& time);

public:

	ResultScoreUI(const float& time);

	~ResultScoreUI();

	

	void Update();
	
	void Draw();

};