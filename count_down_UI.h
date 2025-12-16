#pragma once

class NormalSubScreen;
class Font;
class SoundBase;


class CountDownUI
{
private:

	const int kParamMax = 255;

	std::shared_ptr<NormalSubScreen> screen_;
	std::shared_ptr<NormalSubScreen> start_screen_;
	std::shared_ptr<Font> font_;
	std::shared_ptr<SoundBase> start_sound_;

	VECTOR pos_;

	int screen_width_;
	int screen_height_;

	int start_ui_screen_width_;
	int start_ui_screen_height_;

	int param_;

	int before_num_;

public:

	CountDownUI();

	~CountDownUI();

	void Update(const float& time);

	void Draw();

};