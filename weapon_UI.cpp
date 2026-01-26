#include"weapon_UI.h"
#include"screen.h"
#include"FPS.h"
#include"input.h"
#include"weapon_checker.h"
#include"gauss.h"
#include"gauss_data.h"
#include"super_attack_state_getter.h"

WeaponUI::WeaponUI()
{

	//もともとの画面の比率

	float all_screen_size = (kGameWidth + kGameHeight);

	float kScreenWidthPercent = kGameWidth / all_screen_size;
	float kScreenHeightPercent = kGameHeight / all_screen_size;


	int sub_screen_width = 1000;
	int sub_screen_height = 1000;

	sub_screen_width = sub_screen_width * kScreenWidthPercent;
	sub_screen_height = sub_screen_height * kScreenHeightPercent;


	//screenの設定
	sub_screen_		= std::make_shared<NormalSubScreen>(VGet(1000.f, 200.f,0.f), kGameWidth, kGameHeight, static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0,FALSE);
	circle_gauss_		= std::make_shared<NormalSubScreen>(VGet(1000.f, 200.f, 0.f), kGameWidth, kGameHeight, static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0,FALSE);
	//blur_circle_ = std::make_shared<NormalSubScreen>(VGet(1000.f, 200.f, 0.f), kGameWidth, kGameHeight, static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0, FALSE);
	//ここでいろんなデーターダウンロード
	bat_button_.handle					= kXButtonHandle;
	warprod_button_.handle				= kYButtonHandle;	

	//posの設定
	bat_button_.pos							= VGet(800.f, 400.f, 0.f);
	warprod_button_.pos					= VGet(1000.f, 200.f, 0.f);

	//元の画像のサイズ
	bat_button_.original_width				= 1920.f;
	bat_button_.original_height			= 1080.f;
	warprod_button_.original_width		= 1920.f;
	warprod_button_.original_height		= 1080.f;

	//どのくらいの大きさにしたいか
	bat_button_.width							= 600.f;
	bat_button_.height							= 600.f;
	warprod_button_.width					= 600.f;
	warprod_button_.height					= 600.f;

	//さいず
	OffsetGraphSize(bat_button_);
	OffsetGraphSize(warprod_button_);

	//ぼかしたサークルの位置
	circle_gauss_pos_ = VGet(bat_button_.pos.x, bat_button_.pos.y, 0.f);
	circle_gauss_r_ = 150.f;

	blur_circle_r_ = kBlurInitRadius;
	blur_circle_param_ = 0;

	if (kSuperAttackGaugeFrameHandle == -1 ||bat_button_.handle == -1 || warprod_button_.handle == -1)
	{
		printfDx("読み込み失敗\n");
	}

	sub_screen_->SetIsDisp(TRUE);
	circle_gauss_->SetIsDisp(TRUE);

	bat_pos_ = kInitBatPos;
	warprod_pos_ = kInitWarprodPos;
	bat_scale_ = kInitBatScale;
	warprod_scale_ = kInitWarprodScale;

	bat_vibration_rad_ = 0.f;
	warprod_vibration_rad_ = 0.f;

	back_circle_color_red_		= 255;
	back_circle_color_green_	= 255;
	back_circle_color_blue_		= 255;

	is_change_red_		= TRUE;
	is_change_green_	= TRUE;
	is_change_blue_		= TRUE;

	SetModelMatrix(kBatHandle, kBatRot, bat_scale_, bat_pos_);
	SetModelMatrix(kBatHandle, kBatRot, bat_scale_, warprod_pos_);
	//gausser_->Update(VGet(kGameWidth * 0.5f, kGameHeight * 0.5f, 0.f), kGameWidth, kGameHeight, circle_gauss_->GetHandle(), 8, 10000);
}

WeaponUI::~WeaponUI()
{
	DeleteGraph(kXButtonHandle);
	DeleteGraph(kYButtonHandle);
	DeleteGraph(kOneKeyHandle);
	DeleteGraph(kTwoKeyHandle);
	DeleteGraph(kSuperAttackGaugeFrameHandle);
	MV1DeleteModel(kBatHandle);
	MV1DeleteModel(kWarprodHandle);
}


void WeaponUI::SetCircle()
{
	switch (WeaponChecker::GetInstance().GetName())
	{
	case WeaponName::kBat:

		circle_gauss_pos_ = bat_button_.pos;
		
		//白色に
		back_circle_color_red_ = 255;
		back_circle_color_green_ = 255;
		back_circle_color_blue_ = 255;

		circle_gauss_r_ = 150;

		break;


	case WeaponName::kBugNet:

		circle_gauss_pos_ = warprod_button_.pos;
		
		// この時必殺技が有効ならば虹色に
		if (SuperAttackStateGetter::GetInstance().GetState() == SuperAttackState::kReady)
		{

			const float kAllSpeed = 3.f;

			// 虹色にする
			const float kChangeCircleColorRedSpeed		= 1.f * kAllSpeed;
			const float kChangeCircleColorGreenSpeed	= 3.f * kAllSpeed;
			const float kChangeCircleColorBlueSpeed		= 5.f * kAllSpeed;

			ChangeColorNum(back_circle_color_red_, is_change_red_, kChangeCircleColorRedSpeed);			//赤
			ChangeColorNum(back_circle_color_green_, is_change_green_, kChangeCircleColorGreenSpeed);	//緑
			ChangeColorNum(back_circle_color_blue_, is_change_blue_, kChangeCircleColorBlueSpeed);		//青
			circle_gauss_r_ = 200;
		}

		break;
	}


}

void WeaponUI::SetClurCircle()
{
	const float kBlurSpeed = 5.f;
	blur_circle_r_ += kBlurSpeed * FPS::GetInstance().GetDeltaTime();
	blur_circle_param_ += static_cast<int>(kBlurSpeed * FPS::GetInstance().GetDeltaTime());
}

void WeaponUI::ChangeColorNum(int& color_num, bool& flag, const float change_speed)
{

	const int kColorNumMax = 255;
	const int kColorNumMin = 120;

	//赤
	if (flag)
	{
		color_num += static_cast<int>(change_speed * FPS::GetInstance().GetDeltaTime());
		if (color_num >= kColorNumMax) { color_num = kColorNumMax; flag = !flag; }
	}
	else
	{
		color_num -= static_cast<int>(change_speed * FPS::GetInstance().GetDeltaTime());
		if (color_num <= kColorNumMin) { color_num = kColorNumMin; flag = !flag; }
	}
}

void WeaponUI::SetWeaponScale()
{
	//サイズをどんくらい大きくするか
	const float kSize = 1.2f;

	switch (WeaponChecker::GetInstance().GetName())
	{
	case WeaponName::kBat:

		bat_scale_ = VScale(kInitBatScale, kSize);
		warprod_scale_ = kInitWarprodScale;

		break;

	case WeaponName::kBugNet:

		warprod_scale_ = VScale(kInitWarprodScale, kSize);
		bat_scale_ = kInitBatScale;
		break;

	}

}

void WeaponUI::SetGraph()
{
	switch (Input::GetInstance().GetDeviceType())
	{
	case InputDeviceType::kKey:

		bat_button_.handle			= kOneKeyHandle;
		warprod_button_.handle		= kTwoKeyHandle;
		
		break;

	case InputDeviceType::kPad:

		bat_button_.handle			= kXButtonHandle;
		warprod_button_.handle		= kYButtonHandle;

		break;
	}


}

void WeaponUI::SetModelMatrix(int handle, const VECTOR& rot, const VECTOR& scale, const VECTOR& pos)
{
	MATRIX rot_mat = MMult(MMult(MGetRotX(rot.x), MGetRotY(rot.y)), MGetRotZ(rot.z));
	MV1SetMatrix(handle, MMult(MMult(MGetScale(scale), rot_mat), MGetTranslate(pos)));
}

void WeaponUI::SetAll()
{
	//UIを上下に浮かすように
	bat_pos_ = UpDown(kInitBatPos, bat_vibration_rad_, kBatVibrationSpeed, kVibrationSize);
	warprod_pos_ = UpDown(kInitWarprodPos, warprod_vibration_rad_, kWarprodVibrationSpeed, kVibrationSize);


	// 武器の種類によって変える
	SetCircle();

	// UIの切り替え
	SetGraph();

	SetWeaponScale();

	SetModelMatrix(kBatHandle, kBatRot, bat_scale_, bat_pos_);
	SetModelMatrix(kWarprodHandle, kWarprodRot, warprod_scale_, warprod_pos_);
}


/*--------public---------*/

void WeaponUI::Update()
{
	SetAll();

	// 周りの丸に虹色のブラーをつける


	//こっからは更新なしにしましょう//

	circle_gauss_->Up();

	DrawCircle(static_cast<int>(circle_gauss_pos_.x), static_cast<int>(circle_gauss_pos_.y), static_cast<int>(circle_gauss_r_),		GetColor(back_circle_color_red_, back_circle_color_green_, back_circle_color_blue_), TRUE);
	//DrawCircle(static_cast<int>(warprod_button_.pos.x), static_cast<int>(warprod_button_.pos.y), static_cast<int>(blur_circle_r_),	GetColor(back_circle_color_red_, back_circle_color_green_, back_circle_color_blue_), FALSE);
	circle_gauss_->Down();

	Gauss::GetInstance().Update(circle_gauss_->GetHandle(), kPixelWidthHigh, kCircleGaussParam);
	
	sub_screen_->Up();			// screenを起動

	DrawUIGraph(bat_button_);
	DrawUIGraph(warprod_button_);

	sub_screen_->SetUpCamera();
	auto light_dir = GetLightDirection();


	SetLightDirection(VGet(0.f, 0.f, 1.f));

	MV1DrawModel(kBatHandle);
	MV1DrawModel(kWarprodHandle);
	
	sub_screen_->SetUpOrignalCamera();
	SetLightDirection(light_dir);
	sub_screen_->Down();		// screenを使わない

	
}


void WeaponUI::Draw()
{
	circle_gauss_->Draw();
	sub_screen_->Draw();
}