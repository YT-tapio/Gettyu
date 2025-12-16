#include<iostream>
#include<string>
#include"DxLib.h"
#include"normal_sub_screen.h"
#include"font.h"
#include"result_score.h"
#include"vector_assistant.h"
#include"color.h"
#include"Draw2D.h"

ResultScoreUI::ResultScoreUI(const float& time)
{
	// 文章を映し出すスクリーン
	const int kSentenceScreenWidth = 700;
	const int kSentenceScreenHeight = 200;

	screen_width_ = kSentenceScreenWidth;
	screen_height_ = kSentenceScreenHeight;

	screen_ = std::make_shared<NormalSubScreen>(VectorAssistant::GetZeroVec(), kSentenceScreenWidth, kSentenceScreenHeight, kSentenceScreenWidth, kSentenceScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	
	const char* kFontPath = "data/font/TanueiKakuPop_1_00/TanueiKakuPosp.otf";
	const char* kFontName = "たぬえいカクポップタイ";
	int kThick = 20;
	int kFontType = DX_FONTTYPE_EDGE;

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
	const float kFastTime = 40.f;
	const char* kFastSentence		= "さいきょうの";
	const char* kFastSentenceDown	= "げっちゅめん";

	if (time < kFastTime)
	{
		sentence_		= kFastSentence;
		down_sentence_	= kFastSentenceDown;
	}

}

void ResultScoreUI::Update()
{
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
	const VECTOR kPos = VectorAssistant::Get2DVec(800, 400);
	Draw2D::ExtendGraph(kPos,screen_width_, screen_height_, screen_->GetHandle(), TRUE);
}

