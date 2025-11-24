#include"screen.h"
#include"enemy_count_UI.h"


EnemyCountUI::EnemyCountUI()
	:enemys_count_(0)
{

	float all_size = (kGameWidth + kGameHeight);

	float width_ratio	= kGameWidth / all_size;
	float height_ratio = kGameHeight / all_size;

	float sub_screen_width		= kWidth * width_ratio;
	float sub_screen_height		= kHeight * height_ratio;

	count_screen_ = std::make_shared<NormalSubScreen>(kInitPos,static_cast<int>(kGameWidth),
		static_cast<int>(kGameHeight),static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height),TRUE,AlphaColorType::kBlack,0,TRUE);

	//screenの起動を行う	
	count_screen_->SetIsDisp(TRUE);


}

EnemyCountUI::~EnemyCountUI()
{

}


/*---private-----*/


void EnemyCountUI::CountDraw()
{
	//残りの敵を受け取る



	int size = GetFontSize();
	SetFontSize(100);
	DrawFormatString(static_cast<int>(kInitCountPos.x), static_cast<int>(kInitCountPos.y), GetColor(0, 255, 255), "%d", enemys_count_);
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
	count_screen_->Draw();
}