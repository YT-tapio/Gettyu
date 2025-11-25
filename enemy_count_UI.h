#pragma once

#include"normal_sub_screen.h"


class EnemyCountUI
{
private:

	//subscreenを用意
	std::shared_ptr<NormalSubScreen> count_screen_;

	const VECTOR kInitPos		= VGet(1000.f, 600.f, 0.f);
	const VECTOR kInitCountPos	= VGet(0.f, 0.f, 0.f);
	
	const float kWidth		= 1000.f;
	const float kHeight		= 1000.f;

	//残りの敵の数を知っておく必要がある
	int* enemys_count_;
	

	void CountDraw();


public:


	EnemyCountUI(int *p);


	~EnemyCountUI();


	void Update();

	void Draw();

};

