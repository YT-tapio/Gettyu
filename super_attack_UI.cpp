#include"super_attack_UI.h"
#include"gauss.h"
#include"gauss_data.h"
#include"super_attack_cool_time.h"
#include"FPS.h"
#include"Draw2D.h"
#include"super_attack_state_getter.h"
#include"input.h"

SuperAttackUI::SuperAttackUI()
{
	float all_screen_size = (kGameWidth + kGameHeight);

	float kScreenWidthPercent = kGameWidth / all_screen_size;
	float kScreenHeightPercent = kGameHeight / all_screen_size;
	
	float sub_screen_width		= 1000;
	float sub_screen_height		= 1000;

	sub_screen_width = sub_screen_width * kScreenWidthPercent;
	sub_screen_height = sub_screen_height * kScreenHeightPercent;

	init_ready_screen_width_ = sub_screen_width;
	init_ready_screen_height_ = sub_screen_height;

	ready_screen_width_ = init_ready_screen_width_;
	ready_screen_height_ = init_ready_screen_height_;

	ready_screen_width_ratio_ = kScreenWidthPercent;
	ready_screen_height_ratio_ = kScreenHeightPercent;

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
	frame_data_.handle				= kGaugeFrameHandle;
	body_data_.handle					= kGaugeBodyHandle;
	back_data_.handle					= kGaugeBackHandle;


	//ここで各ステートいじる
	back_data_.width					= kDispBackWidth;
	back_data_.height					= kDispBackHeight;

	body_data_.width					= kDispBodyWidth;
	body_data_.height					= kDispBodyHeight;

	

	OffsetGraphSize(frame_data_);
	OffsetGraphSize(body_data_);
	OffsetGraphSize(back_data_);

	// maskのデータをつくる
	// ここでは端にする
	gauge_mask_data_.pos		= VGet((frame_data_.pos.x + (frame_data_.width * 0.5f)), (frame_data_.pos.y - (frame_data_.height * 0.5f)), 0.f);
	gauge_mask_data_.width	= 0.f;
	gauge_mask_data_.height	= body_data_.height;
	gauge_mask_data_.color	= kMaskColor;

	//3Dモデルのせってい
	weapon_data_.handle		= kWeaponHandle;
	weapon_data_.pos			= kInitWeaponPos;
	weapon_data_.rot			= kInitWeaponRot;
	weapon_data_.scale		= kInitWeaponScale;
	Set3DModelMatrix(weapon_data_);

	frame_screen_			= std::make_shared<NormalSubScreen>(kInitScreenPos, static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0,FALSE);
	body_screen_			= std::make_shared<NormalSubScreen>(kInitScreenPos, static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 10,TRUE);
	back_screen_			= std::make_shared<NormalSubScreen>(kInitScreenPos, static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0,FALSE);
	weapon_screen_			= std::make_shared<NormalSubScreen>(kInitWeaponScreenPos, static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0, FALSE);
	effect_screen_			= std::make_shared<NormalSubScreen>(kInitScreenPos, static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0, FALSE);
	ready_screen_			= std::make_shared<NormalSubScreen>(kInitScreenPos, static_cast<int>(kGameWidth), static_cast<int>(kGameHeight), static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0, FALSE);
	input_type_screen_		= std::make_shared<NormalSubScreen>(VectorAssistant::GetZeroVec(), static_cast<int>(500), static_cast<int>(500), static_cast<int>(500), static_cast<int>(500), TRUE, AlphaColorType::kBlack, 0, FALSE);

	push_UI_pos_ = VectorAssistant::Get2DVec(250.f, 250.f);

	if (kGaugeBackHandle == -1)
	{
		printfDx("2D:読み込みエラー");
	}

	if (kGaugeBodyHandle == -1)
	{
		printfDx("2D:読み込みエラー");
	}

	if (kGaugeFrameHandle == -1)
	{
		printfDx("2D:読み込みエラー");
	}

	if (kWeaponHandle == -1)
	{
		printfDx("3D:読み込みエラー");
	}

	frame_screen_	->SetIsDisp(TRUE);
	body_screen_	->SetIsDisp(TRUE);
	back_screen_		->SetIsDisp(TRUE);
	weapon_screen_->SetIsDisp(TRUE);
	effect_screen_	->SetIsDisp(TRUE);
	ready_screen_	->SetIsDisp(TRUE);
	input_type_screen_->SetIsDisp(TRUE);
	//どのくらい大きくするかを決定
	frame_target_width_ = 0.f;
	frame_target_height_ = 0.f;
	body_target_width_ = 0.f;
	body_target_height_ = 0.f;
	back_target_width_ = 0.f;
	back_target_height_ = 0.f;

	size_up_count_ = 0;
	size_down_count_ = 0;
	is_size_up_ = FALSE;
	is_size_down_ = FALSE;
	is_ready_size_up = FALSE;
	is_ready_ = FALSE;

	super_attack_ready_param_ = kSuperAttackParamInitNum;
	change_color_num_ = 0;
}


SuperAttackUI::~SuperAttackUI()
{
	DeleteGraph(kGaugeFrameHandle);
	DeleteGraph(kGaugeBodyHandle);
	DeleteGraph(kGaugeBackHandle);
	MV1DeleteModel(kWeaponHandle);
}


void SuperAttackUI::SetMaskSize()
{
	// bodyのwidthをどうにかする

	// superattackのタイマーを受け取る

	float ratio = SuperAttackCoolTime::GetInstance().GetRatio();

	gauge_mask_data_.width = -(body_data_.width * (1.f - ratio));

	if (ratio >= 1.f)
	{
		if (!is_ready_size_up)
		{
			is_size_up_ = TRUE;
			SizeUpInit();
		}
		is_ready_size_up = TRUE;
	}
	else
	{
		is_ready_size_up = FALSE;
	}

}


void SuperAttackUI::SetGaugeSizeUp()
{
	if (is_size_up_)
	{
		SizeUp(frame_data_, is_size_up_, frame_target_width_, frame_target_height_, kSizeUpSpeed, size_up_count_);
		SizeUp(body_data_, is_size_up_, body_target_width_, body_target_height_, kSizeUpSpeed, size_up_count_);
		SizeUp(back_data_, is_size_up_, back_target_width_, back_target_height_, kSizeUpSpeed, size_up_count_);

		size_up_count_++;

		//サイズアップ終了したら
		if (!is_size_up_)
		{
			//
			SizeDownInit();
			is_size_down_ = TRUE;
		}

	}
	else
	{
		size_up_count_ = 0;
	}


}


void SuperAttackUI::SetGaugeSizeDown()
{
	if (is_size_down_)
	{
		SizeDown(frame_data_, is_size_down_, frame_target_width_, frame_target_height_, kSizeDownSpeed, size_down_count_);
		SizeDown(body_data_, is_size_down_, body_target_width_, body_target_height_, kSizeDownSpeed, size_down_count_);
		SizeDown(back_data_, is_size_down_, back_target_width_, back_target_height_, kSizeDownSpeed, size_down_count_);

		size_down_count_++;
	}
	else
	{
		size_down_count_ = 0;
	}
}



void SuperAttackUI::SizeUpInit()
{
	frame_target_width_			= frame_data_.width + (kAddSize * frame_data_.width_ratio);
	frame_target_height_			= frame_data_.height + (kAddSize * frame_data_.height_ratio);

	body_target_width_			= body_data_.width + (kAddSize * body_data_.width_ratio);
	body_target_height_			= body_data_.height + (kAddSize * body_data_.height_ratio);

	back_target_width_			= back_data_.width + (kAddSize * back_data_.width_ratio);
	back_target_height_			= back_data_.height + (kAddSize * back_data_.height_ratio);
}

void SuperAttackUI::SizeDownInit()
{
	frame_target_width_		= frame_data_.width - (kAddSize * frame_data_.width_ratio);
	frame_target_height_		= frame_data_.height - (kAddSize * frame_data_.height_ratio);

	body_target_width_		= body_data_.width - (kAddSize * body_data_.width_ratio);
	body_target_height_		= body_data_.height - (kAddSize * body_data_.height_ratio);

	back_target_width_		= back_data_.width - (kAddSize * back_data_.width_ratio);
	back_target_height_		= back_data_.height - (kAddSize * back_data_.height_ratio);
}

/*-----------public----------*/



void SuperAttackUI::Update()
{
	const float kMaxHue = 180.f;
	const float kChangeHueSpeed = 15.f;
	auto now_state = SuperAttackStateGetter::GetInstance().GetState();

	//更新処理
	SetMaskSize();

	SetGaugeSizeUp();
	
	SetGaugeSizeDown();

	if (now_state == SuperAttackState::kReady) 
	{ 
		float param_speed = (20 * FPS::GetInstance().GetDeltaTime());
		float size_speed = (70 * FPS::GetInstance().GetDeltaTime());

		super_attack_ready_param_ -= param_speed;
		// ready_screen_width_ += ready_screen_width_ratio_ * size_speed;
		// ready_screen_height_ += ready_screen_height_ratio_ * size_speed;
		
		ready_screen_width_ +=  size_speed;
		ready_screen_height_ += size_speed;

		if (super_attack_ready_param_ <= 10)
		{
			super_attack_ready_param_ = kSuperAttackParamInitNum;
			ready_screen_width_ = init_ready_screen_width_;
			ready_screen_height_ = init_ready_screen_height_;
		}
	}

	//ここでがぞうのdraw(screenを起動してから)

	//back_screen
	back_screen_->Up();
	DrawUIGraph(back_data_);
	back_screen_->Down();

	//frame_screen
	frame_screen_->Up();
	DrawUIGraph(frame_data_);
	frame_screen_->Down();

	//body_screen
	body_screen_->Up();

	//今の必殺技の状態がためている状態じゃないときは普通に描画
	if (now_state != SuperAttackState::kCoolTime)
	{
		change_color_num_ += (kChangeHueSpeed * FPS::GetInstance().GetDeltaTime());

		if (change_color_num_ > kMaxHue)
		{
			int a = change_color_num_ - kMaxHue;
			change_color_num_ = -kMaxHue + a;
		}

		Draw2D::ColorChangeGraph(body_data_.pos, body_data_.width, body_data_.height, body_data_.handle, TRUE, change_color_num_);
	}
	else
	{
		Draw2D::ExtendGraph(body_data_.pos, body_data_.width, body_data_.height, body_data_.handle, TRUE);
	}

	DrawMaskBox(gauge_mask_data_);
	body_screen_->Down();

	//武器の表示
	weapon_screen_->Up();
	weapon_screen_->SetUpCamera();
	auto light_dir = GetLightDirection();
	SetLightDirection(VGet(0.f, 0.f, 1.f));
	Draw3DModel(weapon_data_);
	weapon_screen_->SetUpOrignalCamera();
	SetLightDirection(light_dir);
	weapon_screen_->Down();

	//bodyとframeだけを描画する

	ready_screen_->Up();
	Draw2D::ColorChangeGraph(body_data_.pos, body_data_.width, body_data_.height, body_data_.handle, TRUE, change_color_num_);
	DrawUIGraph(frame_data_);
	ready_screen_->Down();

	// 操作方法の描画を行う

	input_type_screen_->Up();

	switch (Input::GetInstance().GetDeviceType())
	{
	case InputDeviceType::kKey:

		Draw2D::ExtendGraph(kSuperAttackPCPos, static_cast<int>(kSuperAttackPCOriginalSize.x * kSuperAttackPCScale.x), 
			static_cast<int>(kSuperAttackPCOriginalSize.y * kSuperAttackPCScale.y), kSuperAttackInputPCButton, TRUE);

			break;

	case InputDeviceType::kPad:

		Draw2D::ExtendGraph(kSuperAttackPadPos, static_cast<int>(kSuperAttackPadOriginalSize.x* kSuperAttackPadScale.x), 
			static_cast<int>(kSuperAttackPadOriginalSize.y * kSuperAttackPadScale.y), kSuperAttackInputPadButton, TRUE);

		break;

	}

	Draw2D::Box(push_UI_pos_, 20, 20, GetColor(0, 0, 0), TRUE);

	DrawString(static_cast<int>(push_UI_pos_.x - 20), static_cast<int>(push_UI_pos_.y - 15), "push", GetColor(255, 255, 255), GetColor(0, 0, 0));


	input_type_screen_->Down();

	Gauss::GetInstance().Update(back_screen_->GetHandle(), kPixelWidthMiddle, kBackGaussParam);
}

void SuperAttackUI::Draw()
{
	back_screen_->Draw();
	
	if (SuperAttackStateGetter::GetInstance().GetState() == SuperAttackState::kReady)
	{
		Draw2D::BlendGraph(kInitScreenPos, ready_screen_width_, ready_screen_height_, ready_screen_->GetHandle(), TRUE, super_attack_ready_param_);
		Draw2D::ExtendGraph(VectorAssistant::Get2DVec(350.f, 140.f), 500, 500, input_type_screen_->GetHandle(), TRUE);
	}

	body_screen_->Draw();			//削れる本体
	frame_screen_->Draw();		//外枠
	weapon_screen_->Draw();
	effect_screen_->Draw();	
}