#include"screen.h"
#include"enemy_count_UI.h"
#include"Draw2D.h"

EnemyCountUI::EnemyCountUI(int *p)
	:enemys_count_(p)
{

	float all_size = (kGameWidth + kGameHeight);

	float width_ratio	= kGameWidth / all_size;
	float height_ratio = kGameHeight / all_size;

	float sub_screen_width		= kWidth * width_ratio;
	float sub_screen_height		= kHeight * height_ratio;

	count_screen_ = std::make_shared<NormalSubScreen>(kInitPos, static_cast<int>(kWidth),
		static_cast<int>(kHeight), static_cast<int>(kWidth), static_cast<int>(kHeight), TRUE, AlphaColorType::kBlack, 0, TRUE);

	//screenの起動を行う	
	count_screen_->SetIsDisp(TRUE);

	param_ = 255;
}

EnemyCountUI::~EnemyCountUI()
{

}


/*---private-----*/


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
	count_screen_->Up();

	// 残りのカウントを描画
	CountDraw();

	count_screen_->Down();
}

void EnemyCountUI::Draw()
{
	if (*enemys_count_ != 0)
	{
		Draw2D::BlendGraph(kInitPos, kWidth, kHeight, count_screen_->GetHandle(), TRUE, param_);
	}
}