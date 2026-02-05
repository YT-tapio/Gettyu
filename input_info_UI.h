#pragma once
#include"vector_assistant.h"

class InputInfoUI
{
private:

	const VECTOR kAllScale = VectorAssistant::Get2DVec(1.4f, 1.4f);

	const VECTOR kInputInfoImagePos	= VectorAssistant::Get2DVec(1050.f,430.f);		// 操作方法が書かれているもの
	const VECTOR kInputInfoButtonPos = VectorAssistant::Get2DVec(50.f, 770.f);		// 対応しているボタン
	const VECTOR kInputTypeInfoPos	= VectorAssistant::Get2DVec(145.f, 770.f);		// そうさほうほう


	const VECTOR kOriginalSize = VectorAssistant::Get2DVec(800.f, 800.f);
	const VECTOR kScale = VectorAssistant::Get2DVec(0.3f * kAllScale.x, 0.3f * kAllScale.y);

	const VECTOR kButtonOriginalSize = VectorAssistant::Get2DVec(200.f, 200.f);		//どのボタンで操作方法を見れるのか
	const VECTOR kButtonScale = VectorAssistant::Get2DVec(0.2f, 0.2f);
	const VECTOR kShutButtonScale = VectorAssistant::Get2DVec(0.1f * kAllScale.x, 0.1f * kAllScale.y);	//とじるぼたん

	const VECTOR kInputTypeInfoOriginalSize = VectorAssistant::Get2DVec(238.f, 63.f);
	const VECTOR kInputTypeInfoScale = VectorAssistant::Get2DVec(0.7f, 0.7f);

	const VECTOR kShutOriginalSize = VectorAssistant::Get2DVec(137.f, 59.f);
	const VECTOR kShutScale = VectorAssistant::Get2DVec(0.45f * kAllScale.x, 0.45f * kAllScale.y);

	const VECTOR kInputInfoShutButtonPos = VAdd(kInputInfoImagePos, VGet(70.f, ((kOriginalSize.y * kScale.y * 0.5f) - 10.f), 0.f));		// 対応しているボタン
	const VECTOR kShutInfoPos = VAdd(kInputInfoShutButtonPos,VGet(38.f,0.f,0.f));				// とじる

	int input_info_handle_;			// 操作方法がかかれているもの
	int button_handle_;				 // 対応しているボタン
	int input_type_info_handle_;	 // そうさほうほう
	int shut_info_handle_;			 // とじる

	float blend_param_;

	bool is_disp_;

	void ChangeParam();

public:

	InputInfoUI();

	~InputInfoUI();

	void Update();

	void Draw();
};
