#pragma once
#define _USE_MATH_DEFINES
#include<math.h>

#include"normal_sub_screen.h"
#include"UI_data.h"
#include"gauss.h"
class WeaponUI
{
private:
	
	//一度がどんくらいか
	const float kOneRad = static_cast<float>(M_PI / 180);

	const float kBatVibrationSpeed = 5;
	const float kWarprodVibrationSpeed = 7;

	const float kVibrationSize = 20.f;

	const VECTOR kInitBatPos				= VGet(850.f, 380.f, 0.f);
	const VECTOR kInitWarprodPos		= VGet(1100.f, 550.f, 0.f);

	const VECTOR kBatRot				= VGet(0.f, kOneRad * 90, kOneRad * 45);
	const VECTOR kWarprodRot		= VGet(kOneRad * 90, kOneRad * 180, kOneRad * -45);

	const VECTOR kInitBatScale				= VGet(2.f, 2.f, 2.f);
	const VECTOR kInitWarprodScale			= VGet(0.8f, 0.8f, 0.8f);

	const int kPixcelWidthHigh			= 32;
	const int kPixcelWidthMiddle		= 16;
	const int kPixcelWidthLow			= 8;

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

	std::shared_ptr<Gauss> gausser_;

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



	void OffsetGraphSize(UIGraphData& graph_data);

	void GraphDraw(UIGraphData graph_data);

	void SetCirclePos();

	void SetWeaponScale();

	void SetGraph();

	void SetModelMatrix(int handle,const VECTOR& rot,const VECTOR& scale,const VECTOR& pos);

	//すべてのsetをおこなうばしょ　
	void SetAll();


	// 上下に揺らす処理(各スピードによって変える)
	VECTOR UpDown(const VECTOR& init_pos, float& rad, float speed, float swing);

public:

	WeaponUI();

	~WeaponUI();

	void Update();

	void Draw();

};