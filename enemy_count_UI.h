#pragma once

#include"normal_sub_screen.h"


class EnemyCountUI
{
private:

	//subscreenを用意
	std::shared_ptr<NormalSubScreen> count_screen_;

	const VECTOR kInitPos		= VGet(1000.f, 750.f, 0.f);
	const VECTOR kInitCountPos	= VGet(10.f, 10.f, 0.f);
	
	const float kWidth		= 300.f;
	const float kHeight		= 100.f;

	//残りの敵の数を知っておく必要がある
	int* enemys_count_;
	float param_;

	void CountDraw();


public:


	EnemyCountUI(int *p);


	~EnemyCountUI();


	void Update();

	void Draw();

};

