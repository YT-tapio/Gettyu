#pragma once
#include"const_rad.h"
#include"vector_assistant.h"

#include"normal_sub_screen.h"
#include"UI_data.h"

class WeaponUI
{
private:
	

	const float kBatVibrationSpeed = 5.f;
	const float kWarprodVibrationSpeed = 7.f;

	const float kVibrationSize = 0.3f;

	const VECTOR kInitBatPos				= VGet(4.2f, -0.5f, 10.f);
	const VECTOR kInitWarprodPos		= VGet(7.8f, 2.5f, 10.f);

	const VECTOR kBatRot				= VGet(0.f, kOneRad * 90, kOneRad * 45);
	const VECTOR kWarprodRot		= VGet(kOneRad * 90, kOneRad * 180, kOneRad * -45);

	const VECTOR kInitBatScale				= VectorAssistant::GetSame3DVec(0.035f);
	const VECTOR kInitWarprodScale			= VectorAssistant::GetSame3DVec(0.015f);


	const int kCircleGaussParam		= 1000;
	
	const int kXButtonHandle			= LoadGraph("data/UI/X_ButtonUI.png");
	const int kYButtonHandle			= LoadGraph("data/UI/Y_ButtonUI.png");
	const int kOneKeyHandle				= LoadGraph("data/UI/KEY_1.png");
	const int kTwoKeyHandle				= LoadGraph("data/UI/KEY_2.png");

	const int kBatHandle					= MV1LoadModel("data/model/weapon/use_path/Bat.mv1");
	const int kWarprodHandle			= MV1LoadModel("data/model/weapon/use_path/Bug_Net3.mv1");

	const int kSuperAttackGaugeFrameHandle = LoadGraph("data/UI/A_ButtonUI.png");

	std::shared_ptr<NormalSubScreen> sub_screen_;
	std::shared_ptr<NormalSubScreen> circle_gauss_;


	VECTOR bat_pos_;
	VECTOR warprod_pos_;

	VECTOR bat_scale_;
	VECTOR warprod_scale_;

	UIGraphData bat_button_;
	UIGraphData warprod_button_;
	
	VECTOR circle_gauss_pos_;
	float circle_gauss_r_;

	float bat_vibration_rad_;
	float warprod_vibration_rad_;


	void SetCirclePos();

	void SetWeaponScale();

	void SetGraph();

	void SetModelMatrix(int handle,const VECTOR& rot,const VECTOR& scale,const VECTOR& pos);

	//Ç∑Ç◊ÇƒÇÃsetÇÇ®Ç±Ç»Ç§ÇŒÇµÇÂÅ@
	void SetAll();


public:

	WeaponUI();

	~WeaponUI();

	void Update();

	void Draw();

};