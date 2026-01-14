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

	//元の画像の大きさ
	const int kOriginalImagWidth		= 793;
	const int kOriginalImageHeight	= 72;
	 
	const int kBackGaussParam			= 1300;

	const int kSuperAttackParamInitNum = 255;

	const VECTOR kInitPos				= VGet(kGameWidth * 0.5f, kGameHeight * 0.5f, 0.f);
	const VECTOR kInitScreenPos		= VGet(200.f, 200.f, 0);

	const VECTOR kInitWeaponScreenPos = VGet(200.f, 150.f, 0.f);
	
	const VECTOR kInitWeaponPos		= VGet(0.7f,0.4f,10.f);
	const VECTOR kInitWeaponScale		= VectorAssistant::GetSame3DVec(0.25f);
	const VECTOR kInitWeaponRot		= VGet(-kOneRad * 30.f, kOneRad * 0.f, kOneRad * 45.f);


	const int disp_width_					= 700;
	const int disp_height_					= 600;

	const int kDispBodyWidth			= 710;
	const int kDispBodyHeight			= 630;

	const int kDispBackWidth			= 730;
	const int kDispBackHeight			= 645;

	const int kAddSize					= 200;

	const float kSizeUpSpeed			= 65.f;
	const float kSizeDownSpeed			= 40.f;


	const float kReadyEffectSpeed		= 1.f;
	const float kReadyEffectSize		= 100.f;
	const float kReadyEffectMaxCount	= 10.f;

	float init_ready_screen_width_;		// 初期画像の大きさ：横
	float init_ready_screen_height_;		// 初期画像の大きさ：縦

	float ready_screen_width_;				// 変更する際の大きさ：横
	float ready_screen_height_;			// 変更する際の大きさ：縦

	float ready_screen_width_ratio_;		// 画像サイズの比率：横
	float ready_screen_height_ratio_;	// 画像サイズの比率：縦

	int frame_target_width_;
	int frame_target_height_;

	int body_target_width_;
	int body_target_height_;

	int back_target_width_;
	int back_target_height_;

	int super_attack_ready_param_;
	int change_color_num_;

	UI3DModelData weapon_data_;

	UIGraphData frame_data_;
	UIGraphData body_data_;
	UIGraphData back_data_;

	MaskData gauge_mask_data_;

	std::shared_ptr<NormalSubScreen> frame_screen_;		// 外枠
	std::shared_ptr<NormalSubScreen> body_screen_;			// 本体
	std::shared_ptr<NormalSubScreen> back_screen_;			// 背景
	std::shared_ptr<NormalSubScreen> weapon_screen_;		// 必殺技の武器を表示
	std::shared_ptr<NormalSubScreen> effect_screen_;		// effectの描画を行う
	std::shared_ptr<NormalSubScreen> button_screen_;		// 対応している操作のボタンを表示
	std::shared_ptr<NormalSubScreen> ready_screen_;		// 準備完了の時の際の画像を表示
	//Effect* ready_effect_;
	
	//UIに沿わす
	VECTOR ready_effect_pos_		= VGet(0.f, 0.f, 0.f);
	VECTOR ready_effect_rot_		= VGet(0.f, 0.f, 0.f);


	bool is_size_up_;
	bool is_size_down_;
	bool is_ready_size_up;
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
