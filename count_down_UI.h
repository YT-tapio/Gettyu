#pragma once

class NormalSubScreen;

class CountDownUI
{
private:

	std::shared_ptr<NormalSubScreen> screen_;

	VECTOR pos_;

	int screen_width_;
	int screen_height_;

	int param_;

public:

	CountDownUI();

	~CountDownUI();

	void Update(const float& time);

	void Draw();

};