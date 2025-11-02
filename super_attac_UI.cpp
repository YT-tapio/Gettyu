#include"super_attack_UI.h"


SuperAttackUI::SuperAttackUI()
{
	float all_screen_size = (kGameWidth + kGameHeight);

	float kScreenWidthPercent = kGameWidth / all_screen_size;
	float kScreenHeightPercent = kGameHeight / all_screen_size;
	
	float sub_screen_width = 1000;
	float sub_screen_height = 1000;

	sub_screen_width = sub_screen_width * kScreenWidthPercent;
	sub_screen_height = sub_screen_height * kScreenHeightPercent;

	

	frame_screen_			= std::make_shared<NormalSubScreen>(VGet(100.f, 50.f, 0), static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0);
	body_screen_			= std::make_shared<NormalSubScreen>(VGet(100.f, 50.f, 0), static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0);
	back_screen_				= std::make_shared<NormalSubScreen>(VGet(100.f, 50.f, 0), static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0);

	if (kGaugeBackHandle == -1)
	{
		printfDx("読み込みエラー");
	}

	if (kGaugeBodyHandle == -1)
	{
		printfDx("読み込みエラー");
	}

	if (kGaugeFrameHandle == -1)
	{
		printfDx("読み込みエラー");
	}

	frame_screen_->SetIsDisp(TRUE);
	body_screen_->SetIsDisp(TRUE);
	back_screen_->SetIsDisp(TRUE);

	disp_width_ = 1500;
	disp_height_ = 1000;

}


SuperAttackUI::~SuperAttackUI()
{
	DeleteGraph(kGaugeFrameHandle);
	DeleteGraph(kGaugeBodyHandle);
	DeleteGraph(kGaugeBackHandle);
}

void SuperAttackUI::GaugeDraw(const int handle)
{
	DrawExtendGraph(static_cast<int>(kCenterPos.x - (disp_width_ * 0.5f)),
		static_cast<int>(kCenterPos.y - (disp_height_ * 0.5f)), static_cast<int>(kCenterPos.x + (disp_width_ * 0.5f)), 
		static_cast<int>(kCenterPos.y + (disp_height_ * 0.5f)),handle, TRUE);
}



void SuperAttackUI::Update()
{
	//更新処理



	//ここでがぞうのdraw(screenを起動してから)

	back_screen_->Up();
	GaugeDraw(kGaugeBackHandle);

	DrawCircle(200, 200, 30, GetColor(255, 255, 255), TRUE);

	back_screen_->Down();

	body_screen_->Up();
	GaugeDraw(kGaugeBodyHandle);
	DrawCircle(350, 350, 30, GetColor(255, 255, 255), TRUE);
	body_screen_->Down();

	frame_screen_->Up();
	GaugeDraw(kGaugeFrameHandle);
	DrawCircle(500, 500, 30, GetColor(255, 255, 255), TRUE);
	frame_screen_->Down();
	
}

void SuperAttackUI::Draw()
{
	back_screen_->Draw();
	body_screen_->Draw();			//削れる本体
	frame_screen_->Draw();		//外枠
}