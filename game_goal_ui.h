#pragma once

class NormalSubScreen;
class ConditionTimer;
class Font;

class GameGoalUI
{
private:

	std::shared_ptr<NormalSubScreen> screen_;

	VECTOR pos_;

	float screen_width_ratio_;
	float screen_height_ratio_;

	int screen_width_;
	int screen_height_;

	int screen_param_;

	int* enemy_num_;

	bool change_offset_;

	std::shared_ptr<ConditionTimer> disp_timer_;
	std::shared_ptr<Font> font_;

	void UpdateScreenSize();

	/// <summary>
	/// Screenに描画するオブジェクトたち
	/// </summary>
	void DrawScreenObject();

public:

	GameGoalUI(int* enemy_num);


	~GameGoalUI();

	void Update();

	void Draw();

};