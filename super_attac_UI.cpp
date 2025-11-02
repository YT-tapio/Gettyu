#include"super_attack_UI.h"


SuperAttackUI::SuperAttackUI()
{
	float all_screen_size = (kGameWidth + kGameHeight);

	float kScreenWidthPercent = kGameWidth / all_screen_size;
	float kScreenHeightPercent = kGameHeight / all_screen_size;
	
	float sub_screen_width		= 1000;
	float sub_screen_height		= 1000;

	sub_screen_width = sub_screen_width * kScreenWidthPercent;
	sub_screen_height = sub_screen_height * kScreenHeightPercent;

	frame_data_.handle = -1;
	frame_data_.original_width = kOriginalImagWidth;
	frame_data_.original_height = kOriginalImageHeight;
	frame_data_.width = disp_width_;
	frame_data_.height = disp_height_;
	
	frame_data_.pos = kInitPos;

	//いったん全部同じように
	body_data_ = frame_data_;
	back_data_ = frame_data_;

	//handleを入れる
	frame_data_.handle = kGaugeFrameHandle;
	body_data_.handle = kGaugeBodyHandle;
	back_data_.handle = kGaugeBackHandle;


	//ここで各ステートいじる
	back_data_.width = kDispBackWidth;
	back_data_.height = kDispBackHeight;

	OffsetGraphSize(frame_data_);
	OffsetGraphSize(body_data_);
	OffsetGraphSize(back_data_);

	frame_screen_			= std::make_shared<NormalSubScreen>(VGet(100.f, 50.f, 0), static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0,FALSE);
	body_screen_			= std::make_shared<NormalSubScreen>(VGet(100.f, 50.f, 0), static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0,FALSE);
	back_screen_				= std::make_shared<NormalSubScreen>(VGet(100.f, 50.f, 0), static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0,FALSE);

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



}


SuperAttackUI::~SuperAttackUI()
{
	DeleteGraph(kGaugeFrameHandle);
	DeleteGraph(kGaugeBodyHandle);
	DeleteGraph(kGaugeBackHandle);
}





void SuperAttackUI::Update()
{
	//更新処理



	//ここでがぞうのdraw(screenを起動してから)

	back_screen_->Up();
	DrawUIGraph(back_data_);
	back_screen_->Down();

	body_screen_->Up();
	DrawUIGraph(body_data_);
	body_screen_->Down();

	frame_screen_->Up();
	DrawUIGraph(frame_data_);
	frame_screen_->Down();
	
}

void SuperAttackUI::Draw()
{
	back_screen_->Draw();
	body_screen_->Draw();			//削れる本体
	frame_screen_->Draw();		//外枠
}