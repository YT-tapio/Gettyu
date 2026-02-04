#include<iostream>
#include<string>
#include"DxLib.h"
#include"normal_sub_screen.h"
#include"font.h"
#include"result_score.h"
#include"color.h"
#include"Draw2D.h"
#include"weapon_UI.h"
#include"UI_data.h"
#include"tutorial.h"

ResultScoreUI::ResultScoreUI(const float& time)
{
	// 文章を映し出すスクリーン
	const int kSentenceScreenWidth = 700;
	const int kSentenceScreenHeight = 200;

	screen_width_ = kSentenceScreenWidth;
	screen_height_ = kSentenceScreenHeight;

	screen_ = std::make_shared<NormalSubScreen>(VectorAssistant::GetZeroVec(), kSentenceScreenWidth, kSentenceScreenHeight, kSentenceScreenWidth, kSentenceScreenHeight, TRUE, AlphaColorType::kBlack, 10, FALSE);
	
	const char* kFontPath = "data/font/TanueiKakuPop_1_00/TanueiKakuPosp.otf";
	const char* kFontName = "たぬえいカクポップタイ";
	int kThick = 20;
	int kFontType = DX_FONTTYPE_EDGE;

	pos_ = kInitPos;
	rad_ = 0.f;

	font_ = std::make_shared<Font>(kFontPath, kFontName, kFontSize, kThick, kFontType);
	sentence_ = "なにもにゅうりょく";
	down_sentence_ = "されてません";
	DecideScore(time);
}

ResultScoreUI::~ResultScoreUI()
{

}

void ResultScoreUI::DecideScore(const float& time)
{

	if (Tutorial::GetInstance().GetIsTutorial()) 
	{
		const char* kTutorialSentence				= "チュートリアル";
		const char* kTutorialSentenceDown		= "かんりょう";
		sentence_				= kTutorialSentence;
		down_sentence_	= kTutorialSentenceDown;
		return;
	}

	const char* kSentenceDown		= "げっちゅめん";

	const float kFastTime = 80.f;
	const char* kFastSentence			= "さいきょうの";

	const float kSecoundTime			= 130.f;
	const char* kSecoundSentence	= "カリスマ";

	const float kThirdTime				= 180.f;
	const char* kThirdSentence			= "いっぱん";

	const char* kBeginnerSentence	= "はじめたて";
	
	down_sentence_ = kSentenceDown;

	if (time < kFastTime)
	{
		sentence_		= kFastSentence;;
		return;
	}

	if (time < kSecoundTime)
	{
		sentence_ = kSecoundSentence;
		return;
	}

	if (time < kThirdTime)
	{
		sentence_ = kThirdSentence;
		return;
	}


	// 最後に何も入力されていないのなら
	sentence_ = kBeginnerSentence;

}

void ResultScoreUI::Update()
{
	//uiをぷかぷかさせる

	const float kSpeed = 15.f;
	const float kSwing = 5.f;

	pos_ = UpDown(kInitPos, rad_, kSpeed, kSwing);

	screen_->Up();

	const char* kDispSentence = sentence_.c_str();
	const char* kDispSentenceDown = down_sentence_.c_str();
	DrawStringToHandle(0, 0, kDispSentence, kFontColor, font_->GetHandle(), GetColor(0, 0, 255));

	int width = kFontSize * 2.f;
	int height = GetDrawStringWidthToHandle("あ", strlen("あ"), font_->GetHandle());

	DrawStringToHandle(width, height, kDispSentenceDown, kFontColor, font_->GetHandle(), GetColor(0, 0, 255));

	screen_->Down();
}

void ResultScoreUI::Draw()
{
	Draw2D::ExtendGraph(pos_,screen_width_, screen_height_, screen_->GetHandle(), TRUE);
}

