#include<memory>
#include"DxLib.h"
#include"tutorial.h"
#include"vector_assistant.h"
#include"Draw2D.h"
#include"font.h"
#include"input.h"
#include"keyconfig.h"
#include"super_attack_state_getter.h"
#include"super_attack_state.h"
#include"situation.h"

Tutorial::Tutorial()
{

}

void Tutorial::SetDeltaTime(float delta_time)
{
	
}

void Tutorial::ChangeTutorial(bool flag)
{
	is_tutorial_ = flag;
}

bool Tutorial::GetIsTutorial()
{
	return is_tutorial_;
}

bool Tutorial::IsInSight(const VECTOR& my_pos, const VECTOR& other_pos, const float& r)
{
	VECTOR dist = VSub(other_pos, my_pos);
	return (VSize(dist) < r);
}

void Tutorial::Awake()
{
	//当たり判定の場所
	weapon_coll_pos_			= VGet(0,10.f,100.f);			// 武器切り替え
	attack_coll_pos_				= VectorAssistant::GetZeroVec();			// 攻撃方法
	super_attack_coll_pos_	= VGet(1.3f,10.f,-162);			// 必殺技

	// 当たり判定を生成
	weapon_info_coll_r_			= 30.f;
	attack_info_coll_r_				= 30.f;
	super_attack_info_coll_r_	= 30.f;

	// タイマーを生成
	const float kDispTimerMax		= 15.f;
	weapon_info_timer_				= std::make_shared<ConditionTimer>(kDispTimerMax);
	attack_info_timer_					= std::make_shared<ConditionTimer>(kDispTimerMax);
	super_attack_info_timer_		= std::make_shared<ConditionTimer>(kDispTimerMax);

	is_disp_weapon_coll_info_			= FALSE;
	is_disp_attack_coll_info_				= FALSE;
	is_disp_super_attack_coll_info_		= FALSE;

	is_disp_ = FALSE;

	secound_weapon_info_ = FALSE;
	third_weapon_info_ = FALSE;

	weapon_info_end_ = FALSE;

	// 文字を描画する際のポジション
	weapon_info_pos_		= VectorAssistant::GetZeroVec();
	attack_info_pos_		= VectorAssistant::GetZeroVec();
	super_attack_info_pos_	= VectorAssistant::GetZeroVec();

	tanuei_font_ = new Font("data/font/TanueiKakuPop_1_00/TanueiKakuPop.otf", "たぬえいカクポップタイ", 40, 5, DX_FONTTYPE_ANTIALIASING_4X4);
}

void Tutorial::Reset()
{
	// タイマーのリセット
	weapon_info_timer_->Reset();
	weapon_info_timer_->Reset();
	weapon_info_timer_->Reset();

	is_disp_weapon_coll_info_			= FALSE;
	is_disp_attack_coll_info_				= FALSE;
	is_disp_super_attack_coll_info_	= FALSE;

	is_disp_ = FALSE;

	secound_weapon_info_		= FALSE;
	third_weapon_info_			= FALSE;
	
	weapon_info_end_ = FALSE;

}

void Tutorial::CheckCollision(const VECTOR& pos)
{
	if (!is_tutorial_) { return; }
	if (!is_disp_weapon_coll_info_)
	{
		is_disp_weapon_coll_info_ = IsInSight(weapon_coll_pos_, pos, weapon_info_coll_r_);
		if (is_disp_weapon_coll_info_) 
		{ 
			is_disp_super_attack_coll_info_	= FALSE;
			secound_weapon_info_				= FALSE;
			third_weapon_info_					= FALSE;
			is_disp_ = TRUE;
			weapon_info_timer_->Reset();
		}

	}

	if (!is_disp_attack_coll_info_)
	{
		is_disp_attack_coll_info_ = IsInSight(attack_coll_pos_,pos,attack_info_coll_r_);
	}

	if (!is_disp_super_attack_coll_info_)
	{
		is_disp_super_attack_coll_info_ = IsInSight(super_attack_coll_pos_, pos, super_attack_info_coll_r_);
		if (is_disp_super_attack_coll_info_) { is_disp_weapon_coll_info_ = FALSE; }
	}

}

void Tutorial::Update()
{
	if (!is_tutorial_) { return; }
	// 当たっているときはその文字を描画する
	if (is_disp_weapon_coll_info_) { weapon_info_timer_->Update(); }

	// 文字の感覚を段々とする

	if (weapon_info_timer_->GetIsEnd() && !weapon_info_end_) 
	{ 
		// さいごもおわったなら描画しない
		if (!secound_weapon_info_ && third_weapon_info_) 
		{ 
			weapon_info_end_ = TRUE; 
			is_disp_ = FALSE; 
			is_disp_weapon_coll_info_ = FALSE;
			return;
		}

		if (!secound_weapon_info_ && !third_weapon_info_)
		{
			secound_weapon_info_ = TRUE;
			weapon_info_timer_->Reset();
		}
		else
		{
			secound_weapon_info_ = FALSE;
			third_weapon_info_ = TRUE;
			weapon_info_timer_->Reset();
		}
		
	}

	if (weapon_info_end_ && SuperAttackStateGetter::GetInstance().GetState() == SuperAttackState::kReady)
	{
		is_disp_super_attack_coll_info_ = TRUE;
		is_disp_weapon_coll_info_ = FALSE;
		//is_disp_ = TRUE;
	}
	/*
	if (weapon_info_end_)
	{
		printfDx("aaa\n");
	}

	*/
	

}

void Tutorial::Draw()
{
	if (!is_tutorial_) { return; }
	VECTOR back_pos			= VAdd(VectorAssistant::GetScreenCenterPos(), VectorAssistant::Get2DVec(0.f, -280.f));

	const int kBoxWidth		= 575.f;
	const int kBoxHeight		= 250.f;

	const int kBoxColor			= GetColor(0, 0, 5);
	const int kBoxAlphaNum = 100;

	int font_color = GetColor(255, 210, 0);
	int font_edge_color = GetColor(255, 69, 0);
	VECTOR center_pos = back_pos;

	const char* kSizeOneSentence = "あ";
	int font_width		= 0;
	int font_height		= 0;

	if (is_disp_|| is_disp_super_attack_coll_info_)
	{
		Draw2D::BlendBox(back_pos, kBoxWidth, kBoxHeight, kBoxColor, TRUE, kBoxAlphaNum);
	}

	
	// ここで文字を描画する

	if (Situation::GetInstance().GetSituationName() < SituationName::kClearOffset)
	{
		if (is_disp_weapon_coll_info_ && !is_disp_super_attack_coll_info_ && !weapon_info_end_)
		{
			if (secound_weapon_info_)
			{
				const char* kFontSentence = "Xボタンのぶきは";
				const char* kFontSentenceMiddle = "てきをすこしのあいだ";
				const char* kFontSentenceDown = "うごけなくできるぞ!";
				font_width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), tanuei_font_->GetHandle());
				font_height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height) * 2.f), kFontSentence, font_color, tanuei_font_->GetHandle(), font_edge_color);

				int font_middle_width = GetDrawStringWidthToHandle(kFontSentenceMiddle, strlen(kFontSentenceMiddle), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_middle_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height) * 0.5f), kFontSentenceMiddle, font_color, tanuei_font_->GetHandle(), font_edge_color);

				int font_down_width = GetDrawStringWidthToHandle(kFontSentenceDown, strlen(kFontSentenceDown), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_down_width) * 0.5f), static_cast<int>(center_pos.y + float(font_height)), kFontSentenceDown, font_color, tanuei_font_->GetHandle(), font_edge_color);
			}
			else if (third_weapon_info_)
			{
				const char* kFontSentence = "Yボタンのぶきは";
				const char* kFontSentenceDown = "てきをつかまえられるぞ!";
				font_width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), tanuei_font_->GetHandle());
				font_height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height) * 1.5f), kFontSentence, font_color, tanuei_font_->GetHandle(), font_edge_color);

				int font_down_width = GetDrawStringWidthToHandle(kFontSentenceDown, strlen(kFontSentenceDown), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_down_width) * 0.5f), static_cast<int>(center_pos.y + float(font_height)), kFontSentenceDown, font_color, tanuei_font_->GetHandle(), font_edge_color);
			}
			else
			{
				const char* kFontSentence = "XとYボタンでぶきをきりかえられるぞ!";
				const char* kFontSentenceDown = "LBでこうげきできるぞ!";
				font_width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), tanuei_font_->GetHandle());
				font_height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height) * 1.5f), kFontSentence, font_color, tanuei_font_->GetHandle(), font_edge_color);

				int font_down_width = GetDrawStringWidthToHandle(kFontSentenceDown, strlen(kFontSentenceDown), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_down_width) * 0.5f), static_cast<int>(center_pos.y + float(font_height)), kFontSentenceDown, font_color, tanuei_font_->GetHandle(), font_edge_color);
			}
		}
		if (is_disp_super_attack_coll_info_)
		{

			if (SuperAttackStateGetter::GetInstance().GetState() == SuperAttackState::kActive)
			{
				const char* kFontSentence = "ひっさつわざちゅうはLTとRTで";
				const char* kFontSentenceEnd = "カメラをうごかせるぞ!";
				font_width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), tanuei_font_->GetHandle());
				font_height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height)), kFontSentence, font_color, tanuei_font_->GetHandle(), font_edge_color);

				int font_end_width = GetDrawStringWidthToHandle(kFontSentenceEnd, strlen(kFontSentenceEnd), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_end_width) * 0.5f), static_cast<int>(center_pos.y + float(font_height)), kFontSentenceEnd, font_color, tanuei_font_->GetHandle(), font_edge_color);
			}
			else
			{
				const char* kFontSentenceZero = "Yのぶきをそうびし";
				const char* kFontSentence = "Rボタンでひっさつわざをうてるぞ!";
				const char* kFontSentenceMiddle = "きょうりょくなぶきにかわり";
				const char* kFontSentenceEnd = "てきをすいこむぞ!";

				font_height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), tanuei_font_->GetHandle());

				int font_zero_width = GetDrawStringWidthToHandle(kFontSentenceZero, strlen(kFontSentenceZero), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_zero_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height) * 2.0f), kFontSentenceZero, font_color, tanuei_font_->GetHandle(), font_edge_color);

				font_width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height) * 1.0f), kFontSentence, font_color, tanuei_font_->GetHandle(), font_edge_color);

				int font_middle_width = GetDrawStringWidthToHandle(kFontSentenceMiddle, strlen(kFontSentenceMiddle), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_middle_width) * 0.5f), static_cast<int>(center_pos.y), kFontSentenceMiddle, font_color, tanuei_font_->GetHandle(), font_edge_color);

				int font_end_width = GetDrawStringWidthToHandle(kFontSentenceEnd, strlen(kFontSentenceEnd), tanuei_font_->GetHandle());
				DrawStringToHandle(static_cast<int>(center_pos.x - float(font_end_width) * 0.5f), static_cast<int>(center_pos.y + float(font_height) * 1.0f), kFontSentenceEnd, font_color, tanuei_font_->GetHandle(), font_edge_color);
			}



		}
	}
	else
	{
		const char* kFontSentence = "ゲームクリア!";

		font_width = GetDrawStringWidthToHandle(kFontSentence, strlen(kFontSentence), tanuei_font_->GetHandle());
		font_height = GetDrawStringWidthToHandle(kSizeOneSentence, strlen(kSizeOneSentence), tanuei_font_->GetHandle());

		DrawStringToHandle(static_cast<int>(center_pos.x - float(font_width) * 0.5f), static_cast<int>(center_pos.y - float(font_height) * 0.5f), kFontSentence, font_color, tanuei_font_->GetHandle(), font_edge_color);
	}

	
	
}

void Tutorial::Debug()
{
	DrawSphere3D(weapon_coll_pos_, weapon_info_coll_r_, 20, GetColor(255, 255, 255), GetColor(255, 255, 255),FALSE);
	DrawSphere3D(attack_coll_pos_, attack_info_coll_r_, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	DrawSphere3D(super_attack_coll_pos_, super_attack_info_coll_r_, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
}

void Tutorial::Delete()
{
	delete tanuei_font_;
}