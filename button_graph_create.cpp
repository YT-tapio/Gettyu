#include<iostream>
#include"DxLib.h"
#include"normal_sub_screen.h"
#include"font.h"
#include"button_graph_create.h"
#include"vector_assistant.h"
#include"Draw2D.h"

ButtonGraph::ButtonGraph()
{

}

void ButtonGraph::MakeStart()
{
	//スクリーンの大きさ
	const float kScreenWidth = 300.f;
	const float kScreenHeight = 80.f;

	//スクリーンの指定
	start_screen_ = new NormalSubScreen(VectorAssistant::GetZeroVec(), kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	start_screen_->SetIsDisp(TRUE);

	VECTOR center_pos	= VectorAssistant::GetHerf(VectorAssistant::Get2DVec(kScreenWidth, kScreenHeight));
	
	int font_color				= GetColor(255, 210, 0);
	int font_edge_color		= GetColor(255, 69, 0);
	int back_color			= GetColor(210, 180, 140);
	int edge_color			= GetColor(255, 215, 0);
	
	const char* kFontSentence = "すたーと";

	start_screen_->Up();

	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, back_color, TRUE);
	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, edge_color, FALSE);

	//フォントの大きさを受け取ります
	int width	= GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), font_->GetHandle());
	const char* kSizeOneSentence = "あ";
	int height	= GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), font_->GetHandle());
	DrawStringToHandle(static_cast<int>(center_pos.x - float(width) * 0.5f), static_cast<int>(center_pos.y - float(height * 0.5f)), kFontSentence, font_color, font_->GetHandle(),font_edge_color);

	start_screen_->Down();
}

void ButtonGraph::MakeInputType()
{
	const float kScreenWidth		= 300.f;
	const float kScreenHeight	= 80.f;


	input_type_screen_ = new NormalSubScreen(VectorAssistant::GetZeroVec(), kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	input_type_screen_->SetIsDisp(TRUE);

	VECTOR center_pos = VectorAssistant::GetHerf(VectorAssistant::Get2DVec(kScreenWidth, kScreenHeight));

	int font_color				= GetColor(255, 210, 0);
	int font_edge_color		= GetColor(255, 69, 0);
	int back_color			= GetColor(210, 180, 140);
	int edge_color			= GetColor(255, 215, 0);

	const char* kFontSentence = "そうさほうほう";

	input_type_screen_->Up();

	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, back_color, TRUE);
	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, edge_color, FALSE);

	int width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), font_->GetHandle());
	const char* kSizeOneSentence = "あ";
	int height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), font_->GetHandle());
	DrawStringToHandle(static_cast<int>(center_pos.x - float(width) * 0.5f), static_cast<int>(center_pos.y - float(height) * 0.5f), kFontSentence, font_color, font_->GetHandle(), font_edge_color);

	input_type_screen_->Down();
}

void ButtonGraph::MakeGoTutorial()
{
	const float kScreenWidth = 300.f;
	const float kScreenHeight = 80.f;


	input_type_screen_ = new NormalSubScreen(VectorAssistant::GetZeroVec(), kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	input_type_screen_->SetIsDisp(TRUE);

	VECTOR center_pos = VectorAssistant::GetHerf(VectorAssistant::Get2DVec(kScreenWidth, kScreenHeight));

	int font_color = GetColor(255, 210, 0);
	int font_edge_color = GetColor(255, 69, 0);
	int back_color = GetColor(210, 180, 140);
	int edge_color = GetColor(255, 215, 0);

	const char* kFontSentence = "チュートリアル";

	input_type_screen_->Up();

	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, back_color, TRUE);
	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, edge_color, FALSE);

	int width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), font_->GetHandle());
	const char* kSizeOneSentence = "あ";
	int height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), font_->GetHandle());
	DrawStringToHandle(static_cast<int>(center_pos.x - float(width) * 0.5f), static_cast<int>(center_pos.y - float(height) * 0.5f), kFontSentence, font_color, font_->GetHandle(), font_edge_color);
}

void ButtonGraph::MakeExit()
{
	const float kScreenWidth = 300.f;
	const float kScreenHeight = 80.f;

	
	exit_screen_ = new NormalSubScreen(VectorAssistant::GetZeroVec(), kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	exit_screen_->SetIsDisp(TRUE);

	VECTOR center_pos	= VectorAssistant::GetHerf(VectorAssistant::Get2DVec(kScreenWidth, kScreenHeight));
	
	int font_color				= GetColor(255, 210, 0);
	int font_edge_color		= GetColor(255, 69, 0);
	int back_color			= GetColor(210, 180, 140);
	int edge_color			= GetColor(255, 215, 0);

	const char* kFontSentence = "げーむをやめる";

	exit_screen_->Up();

	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, back_color, TRUE);
	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, edge_color, FALSE);

	int width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), font_->GetHandle());
	const char* kSizeOneSentence = "あ";
	int height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), font_->GetHandle());
	DrawStringToHandle(static_cast<int>(center_pos.x - float(width) * 0.5f), static_cast<int>(center_pos.y - float(height) * 0.5f), kFontSentence, font_color, font_->GetHandle(), font_edge_color);

	exit_screen_->Down();
}

void ButtonGraph::MakeRetryScreen()
{
	const float kScreenWidth = 230.f;
	const float kScreenHeight = 100.f;

	retry_screen_ = new NormalSubScreen(VectorAssistant::GetZeroVec(), kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	retry_screen_->SetIsDisp(TRUE);


	VECTOR center_pos = VectorAssistant::GetHerf(VectorAssistant::Get2DVec(kScreenWidth, kScreenHeight));

	int font_color = GetColor(255, 210, 0);
	int font_edge_color = GetColor(255, 69, 0);
	int back_color = GetColor(210, 180, 140);
	int edge_color = GetColor(255, 215, 0);

	const char* kFontSentence = "もういちど";

	retry_screen_->Up();

	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, back_color, TRUE);
	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, edge_color, FALSE);

	int width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), font_->GetHandle());
	const char* kSizeOneSentence = "あ";
	int height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), font_->GetHandle());
	DrawStringToHandle(static_cast<int>(center_pos.x - float(width) * 0.5f), static_cast<int>(center_pos.y - float(height) * 0.5f), kFontSentence, font_color, font_->GetHandle(), font_edge_color);

	retry_screen_->Down();
}

void ButtonGraph::MakeGoTitle()
{
	const float kScreenWidth	= 230.f;
	const float kScreenHeight	= 100.f;

	go_title_screen_ = new NormalSubScreen(VectorAssistant::GetZeroVec(), kScreenWidth, kScreenHeight, kScreenWidth, kScreenHeight, FALSE, AlphaColorType::kBlack, 10, FALSE);
	go_title_screen_->SetIsDisp(TRUE);


	VECTOR center_pos	= VectorAssistant::GetHerf(VectorAssistant::Get2DVec(kScreenWidth, kScreenHeight));
	
	int font_color				= GetColor(255, 210, 0);
	int font_edge_color		= GetColor(255, 69, 0);
	int back_color			= GetColor(210, 180, 140);
	int edge_color			= GetColor(255, 215, 0);
	
	const char* kFontSentence = "たいとるへ";

	go_title_screen_->Up();

	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, back_color, TRUE);
	Draw2D::Box(center_pos, kScreenWidth, kScreenHeight, edge_color, FALSE);

	int width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), font_->GetHandle());
	const char* kSizeOneSentence = "あ";
	int height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), font_->GetHandle());
	DrawStringToHandle(static_cast<int>(center_pos.x - float(width) * 0.5f), static_cast<int>(center_pos.y - float(height) * 0.5f), kFontSentence, font_color, font_->GetHandle(), font_edge_color);

	go_title_screen_->Down();
}

void ButtonGraph::MakeGraph()
{
	// フォントの生成
	const char* kFontPath = "data/font/TanueiKakuPop_1_00/TanueiKakuPop.otf";
	const char* kFontName = "たぬえいカクポップタイ";

	const float kFontSize			= 50.f;
	const float kFontThick			= 20.f;
	const int kFontType				= DX_FONTTYPE_EDGE;

	font_ = new Font(kFontPath, kFontName, kFontSize, kFontThick, kFontType);

	MakeStart();
	MakeInputType();
	MakeGoTutorial();
	MakeExit();
	MakeRetryScreen();
	MakeGoTitle();
}

void ButtonGraph::DeleteGraph()
{
	delete font_;
	delete start_screen_;
	delete go_tutorial_screen_;
	delete input_type_screen_;
	delete exit_screen_;
	delete go_title_screen_;
}

const int ButtonGraph::GetStartHandle() const
{
	return start_screen_->GetHandle();
}

const int ButtonGraph::GetInputTypeHandle() const
{
	return input_type_screen_->GetHandle();
}

const int ButtonGraph::GetGoTutorialHandle() const
{
	return go_tutorial_screen_->GetHandle();
}

const int ButtonGraph::GetExitHandle() const
{
	return exit_screen_->GetHandle();
}

const int ButtonGraph::GetRetryHandle() const
{
	return retry_screen_->GetHandle();
}

const int ButtonGraph::GetGoTitleHandle() const
{
	return go_title_screen_->GetHandle();
}