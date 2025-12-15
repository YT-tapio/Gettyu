#pragma once
#include<iostream>
#include"screen.h"
#include"UI_data.h"
#include"mask.h"
#include"normal_sub_screen.h"
#include"const_rad.h"
#include"effect.h"
#include"EffekseerForDXLib.h"
#include"vector_assistant.h"

class SuperAttackUI
{
private:

	const int kMaskColor = GetColor(0, 0, 0);

	const int kGaugeFrameHandle		= LoadGraph("data/UI/super_attack/GaugeFreamOutsideB_Orende.png");
	const int kGaugeBodyHandle		= LoadGraph("data/UI/super_attack/BodyGaugeB_Green.png");
	const int kGaugeBackHandle		= LoadGraph("data/UI/super_attack/BodyBackB_Black.png");

	const int kWeaponHandle = MV1LoadModel("data/model/weapon/vacuum/vacuum.mv1");

	//Œ³‚Ì‰æ‘œ‚Ì‘å‚«‚³
	const int kOriginalImagWidth		= 793;
	const int kOriginalImageHeight	= 72;
	 
	const int kBackGaussParam			= 1300;

	const VECTOR kInitPos				= VGet(kGameWidth * 0.5f, kGameHeight * 0.5f, 0.f);
	const VECTOR kInitScreenPos		= VGet(200.f, 150.f, 0);

	const VECTOR kInitWeaponScreenPos = VGet(200.f, 100.f, 0.f);
	
	const VECTOR kInitWeaponPos		= VGet(1.2f,0.4f,10.f);
	const VECTOR kInitWeaponScale		= VectorAssistant::GetSame3DVec(0.25f);
	const VECTOR kInitWeaponRot		= VGet(-kOneRad * 30.f, kOneRad * 0.f, kOneRad * 45.f);


	const int disp_width_					= 700;
	const int disp_height_					= 600;

	const int kDispBodyWidth			= 710;
	const int kDispBodyHeight			= 605;

	const int kDispBackWidth			= 730;
	const int kDispBackHeight			= 620;

	const int kAddSize					= 200;

	const float kSizeUpSpeed		= 65.f;
	const float kSizeDownSpeed	= 40.f;


	const float kReadyEffectSpeed			= 1.f;
	const float kReadyEffectSize			= 100.f;
	const float kReadyEffectMaxCount	= 10.f;

	int frame_target_width_;
	int frame_target_height_;

	int body_target_width_;
	int body_target_height_;

	int back_target_width_;
	int back_target_height_;

	UI3DModelData weapon_data_;

	UIGraphData frame_data_;
	UIGraphData body_data_;
	UIGraphData back_data_;

	MaskData gauge_mask_data_;

	std::shared_ptr<NormalSubScreen> frame_screen_;		// ŠO˜g
	std::shared_ptr<NormalSubScreen> body_screen_;			// –{‘Ì
	std::shared_ptr<NormalSubScreen> back_screen_;			// ”wŒi
	std::shared_ptr<NormalSubScreen> weapon_screen_;		// •KE‹Z‚Ì•Ší‚ğ•\¦
	std::shared_ptr<NormalSubScreen> effect_screen_;		// effect‚Ì•`‰æ‚ğs‚¤
	std::shared_ptr<NormalSubScreen> button_screen_;		// ‘Î‰‚µ‚Ä‚¢‚é‘€ì‚Ìƒ{ƒ^ƒ“‚ğ•\¦

	//Effect* ready_effect_;
	
	//UI‚É‰ˆ‚í‚·
	VECTOR ready_effect_pos_		= VGet(0.f, 0.f, 0.f);
	VECTOR ready_effect_rot_		= VGet(0.f, 0.f, 0.f);


	bool is_size_up_;
	bool is_size_down_;
	bool is_ready_;

	int size_up_count_;
	int size_down_count_;

	void SetMaskSize();

	void SetGaugeSizeUp();

	void SetGaugeSizeDown();

	void SizeUpInit();

	void SizeDownInit();

public:

	SuperAttackUI();

	~SuperAttackUI();

	void Update();

	void Draw();

};
