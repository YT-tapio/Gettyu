#include<math.h>
#define _USE_MATH_DEFINES

#include"screen.h"
#include"enemy_count_UI.h"
#include"Draw2D.h"
#include"FPS.h"
#include"const_rad.h"
#include"UI_data.h"

EnemyCountUI::EnemyCountUI(int *p)
	:enemys_count_(p)
{

	float all_size = (kGameWidth + kGameHeight);

	float width_ratio	= kGameWidth / all_size;
	float height_ratio = kGameHeight / all_size;

	float sub_screen_width		= kWidth * width_ratio;
	float sub_screen_height		= kHeight * height_ratio;

	count_screen_ = std::make_shared<NormalSubScreen>(kScreenInitPos, static_cast<int>(kWidth),
		static_cast<int>(kHeight), static_cast<int>(kWidth), static_cast<int>(kHeight), TRUE, AlphaColorType::kBlack, 0, TRUE);

	screen_pos_ = kScreenInitPos;

	//screenの起動を行う	
	count_screen_->SetIsDisp(TRUE);

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
	int size = GetFontSize();
	SetFontSize(60);
	DrawString(static_cast<int>(kInitCountPos.x), static_cast<int>(kInitCountPos.y), "のこり", GetColor(0, 255, 255));
	SetFontSize(100);
	DrawFormatString(static_cast<int>(kInitCountPos.x + 190), static_cast<int>(kInitCountPos.y - 20), GetColor(0, 255, 255), "%d", *enemys_count_);
	SetFontSize(size);
}



/*-----public----*/

void EnemyCountUI::Update()
{
	UpdateUiPos();
	// UpdateDispParam();

	count_screen_->Up();
	// 残りのカウントを描画
	CountDraw();
	count_screen_->Down();
}

void EnemyCountUI::Draw()
{
	if (*enemys_count_ != 0)
	{
		Draw2D::BlendGraph(screen_pos_, kWidth, kHeight, count_screen_->GetHandle(), TRUE, param_);
	}
}