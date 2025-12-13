#pragma once

#include"normal_sub_screen.h"

class Font;

class EnemyCountUI
{
private:

	const VECTOR kScreenInitPos		= VGet(1000.f, 750.f, 0.f);
	const VECTOR kInitCountPos	= VGet(10.f, 10.f, 0.f);
	
	VECTOR screen_pos_;

	const float kWidth		= 300.f;
	const float kHeight		= 100.f;

	const float kEnemyCountScreenWidth		= 100.f;
	const float kEnemyCountScreenHeight		= 100.f;

	const int kFontColor = GetColor(255, 255, 15);

	//subscreenを用意
	std::shared_ptr<NormalSubScreen> all_screen_;
	std::shared_ptr<NormalSubScreen> enemy_count_screen_;
	std::shared_ptr<Font> count_font_;

	//残りの敵の数を知っておく必要がある
	int* enemys_count_;
	float param_;

	bool is_disp_;

	void UpdateDispParam();

	void UpdateUiPos();

	void CountDraw();

public:


	EnemyCountUI(int *p);


	~EnemyCountUI();


	void Update();

	void Draw();

};

