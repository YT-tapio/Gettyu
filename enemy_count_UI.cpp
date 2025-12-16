#include<math.h>
#define _USE_MATH_DEFINES

#include"screen.h"
#include"font.h"
#include"enemy_count_UI.h"
#include"Draw2D.h"
#include"FPS.h"
#include"const_rad.h"
#include"UI_data.h"


EnemyCountUI::EnemyCountUI(int *p)
	:enemys_count_(p)
{
	const char* kFontFile	= "data/fontTanueiKakuPop_1_00/TanueiKakuPop.otf";
	const char* kFontName	= "たぬえいカクポップタイ";

	const int kFontSize			= 50;
	const int kFontThick		= 20;
	const int kFontType		= DX_FONTTYPE_EDGE;

	float all_size			= (kGameWidth + kGameHeight);

	float width_ratio		= kGameWidth / all_size;
	float height_ratio		= kGameHeight / all_size;

	float sub_screen_width		= kWidth * width_ratio;
	float sub_screen_height		= kHeight * height_ratio;

	all_screen_ = std::make_shared<NormalSubScreen>(kScreenInitPos, static_cast<int>(kWidth),
		static_cast<int>(kHeight), static_cast<int>(kWidth), static_cast<int>(kHeight), TRUE, AlphaColorType::kBlack, 0, TRUE);

	enemy_count_screen_ = std::make_shared<NormalSubScreen>(kScreenInitPos, static_cast<int>(kFontSize),
		static_cast<int>(kFontSize), static_cast<int>(kFontSize), static_cast<int>(kFontSize), TRUE, AlphaColorType::kBlack, 0, TRUE);

	count_font_ = std::make_shared<Font>(kFontFile, kFontName, kFontSize, kFontThick, kFontType);

	screen_pos_ = kScreenInitPos;

	//screenの起動を行う	
	all_screen_->SetIsDisp(TRUE);
	enemy_count_screen_->SetIsDisp(TRUE);
	is_disp_ = FALSE;
	param_ = 255;
}

EnemyCountUI::~EnemyCountUI()
{

}


/*---private-----*/

void EnemyCountUI::UpdateDispParam()
{
	if (is_disp_)
	{
		const float kParamOffsetSpeed = 10.f;

		param_ -= kParamOffsetSpeed * FPS::GetInstance().GetDeltaTime();
		if (param_ < 0)
		{
			param_ = 255;
			is_disp_ = FALSE;
		}

	}
}

void EnemyCountUI::UpdateUiPos()
{
	// uiがアップダウンするやつを作ります
	const float kSpeed = 50.f;
	const float kSwing = 10.f;

	static float rad = 0.f;
	
	screen_pos_ = UpDown(kScreenInitPos,rad,kSpeed,kSwing);
}

void EnemyCountUI::CountDraw()
{
	//残りの敵を受け取る
	DrawStringToHandle(static_cast<int>(kInitCountPos.x), static_cast<int>(kInitCountPos.y), kRestUI, kFontColor, count_font_->GetHandle());
}

/*-----public----*/

void EnemyCountUI::Update()
{
	UpdateUiPos();
	// UpdateDispParam();

	all_screen_->Up();
	// 残りのカウントを描画
	CountDraw();
	all_screen_->Down();

	enemy_count_screen_->Up();
	DrawFormatStringToHandle(0, 0, kFontColor, count_font_->GetHandle(), "%d", *enemys_count_);
	enemy_count_screen_->Down();

}

void EnemyCountUI::Draw()
{
	if (*enemys_count_ != 0)
	{
		Draw2D::BlendGraph(screen_pos_, kWidth, kHeight, all_screen_->GetHandle(), TRUE, param_);
		int width = GetDrawStringWidth(kRestUI, strlen(kRestUI), count_font_->GetHandle());
		Draw2D::BlendGraph(VAdd(screen_pos_, VGet(float(width) + float(kEnemyCountScreenWidth) * 0.4f, float(kEnemyCountScreenHeight) * -0.5f, 0.f)), kEnemyCountScreenWidth, kEnemyCountScreenHeight, enemy_count_screen_->GetHandle(), TRUE, param_);
	}
}