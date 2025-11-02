#pragma once
#include<iostream>
#include"screen.h"
#include"normal_sub_screen.h"

class SuperAttackUI
{
private:

	const int kGaugeFrameHandle		= LoadGraph("data/UI/super_attack/GaugeFreamOutsideB_Orende.png");
	const int kGaugeBodyHandle		= LoadGraph("data/UI/super_attack/BodyGaugeB_Green.png");
	const int kGaugeBackHandle		= LoadGraph("data/UI/super_attack/BodyBackB_BlackTranslucent.png");

	int disp_width_ = 500;
	int disp_height_ = 300;

	const VECTOR kCenterPos = VGet(kGameWidth * 0.5f, kGameHeight * 0.5f, 0.f);
	

	std::shared_ptr<NormalSubScreen> frame_screen_;
	std::shared_ptr<NormalSubScreen> body_screen_;
	std::shared_ptr<NormalSubScreen> back_screen_;


	void GaugeDraw(const int handle);

public:

	SuperAttackUI();

	~SuperAttackUI();

	void Update();

	void Draw();

};
