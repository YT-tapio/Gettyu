#pragma once

class NormalSubScreen;
class Font;

class ResultScoreUI
{
private:

	const int kFontColor = GetColor(255, 215, 0);
	const int kFontSize = 100;

	std::string sentence_;
	std::string down_sentence_;
	std::shared_ptr<NormalSubScreen> screen_;
	std::shared_ptr<Font> font_;
	

	int screen_width_;
	int screen_height_;

	void DecideScore(const float& time);

public:

	ResultScoreUI(const float& time);

	~ResultScoreUI();

	

	void Update();
	
	void Draw();

};