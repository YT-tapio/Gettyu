#pragma once

#include"condition_timer.h"

class Font;

class Tutorial
{
private:

	bool is_tutorial_ = FALSE;

	VECTOR weapon_coll_pos_;			// •ŠíØ‚è‘Ö‚¦
	VECTOR attack_coll_pos_;			// UŒ‚•û–@
	VECTOR super_attack_coll_pos_;		// •KE‹Z

	float weapon_info_coll_r_;
	float attack_info_coll_r_;
	float super_attack_info_coll_r_;

	std::shared_ptr<ConditionTimer> weapon_info_timer_;				// •ŠíØ‚è‘Ö‚¦•`‰æ
	std::shared_ptr<ConditionTimer> attack_info_timer_;				// UŒ‚•û–@‚Ì•`‰æŠÔ
	std::shared_ptr<ConditionTimer> super_attack_info_timer_;		// •ŠíØ‚è‘Ö‚¦•`‰æ

	Font* tanuei_font_;

	bool is_disp_weapon_coll_info_			= FALSE;
	bool is_disp_attack_coll_info_				= FALSE;
	bool is_disp_super_attack_coll_info_	= FALSE;

	bool is_disp_ = FALSE;

	bool secound_weapon_info_	= FALSE;
	bool third_weapon_info_			= FALSE;

	bool weapon_info_end_ = FALSE;

	// •`‰æ‚·‚éÛ‚Ì•¶š‚ÌêŠ
	VECTOR weapon_info_pos_;
	VECTOR attack_info_pos_;
	VECTOR super_attack_info_pos_;

	

	Tutorial();

	bool IsInSight(const VECTOR& my_pos, const VECTOR& other_pos, const float& r);

public:

	static Tutorial& GetInstance()
	{
		static Tutorial instance;
		return instance;
	}

	void SetDeltaTime(float delta_time);

	void ChangeTutorial(bool flag);

	bool GetIsTutorial();

	void Awake();

	void Reset();

	void CheckCollision(const VECTOR& pos);

	void Update();

	// ‚±‚±‚Åà–¾‚ğ•`‰æ‚³‚¹‚é
	void Draw();

	void Debug();

	void Delete();

};