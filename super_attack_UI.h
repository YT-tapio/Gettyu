#pragma once
#include<iostream>
#include"screen.h"
#include"UI_data.h"
#include"normal_sub_screen.h"

class SuperAttackUI
{
private:

	const int kGaugeFrameHandle		= LoadGraph("data/UI/super_attack/GaugeFreamOutsideB_Orende.png");
	const int kGaugeBodyHandle		= LoadGraph("data/UI/super_attack/BodyGaugeB_Green.png");
	const int kGaugeBackHandle		= LoadGraph("data/UI/super_attack/BodyBackB_BlackTranslucent.png");

	//Œ³‚Ì‰æ‘œ‚Ì‘å‚«‚³
	const int kOriginalImagWidth = 793;
	const int kOriginalImageHeight = 72;

	const VECTOR kInitPos = VGet(kGameWidth * 0.5f, kGameHeight * 0.5f, 0.f);

	const int disp_width_ = 700;
	const int disp_height_ = 600;

	const int kDispBackWidth = 750;
	const int kDispBackHeight = 650;

	UIGraphData frame_data_;
	UIGraphData body_data_;
	UIGraphData back_data_;
	

	std::shared_ptr<NormalSubScreen> frame_screen_;
	std::shared_ptr<NormalSubScreen> body_screen_;
	std::shared_ptr<NormalSubScreen> back_screen_;

public:

	SuperAttackUI();

	~SuperAttackUI();

	void Update();

	void Draw();

};
