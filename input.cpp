#include"DxLib.h"
#include<math.h>
#include"screen.h"
#include"Calculation.h"
#include"input.h"
#include"debug.h"



Input::Input()
	:device_type_(InputDeviceType::kNothing)
{

}


/*-----------private----------*/


bool Input::GetInputKey()
{
	if (CheckHitKeyAll(DX_CHECKINPUT_KEY) != 0 ||
		CheckHitKeyAll(DX_CHECKINPUT_MOUSE) != 0)
	{
		return TRUE;
	}

	return FALSE;
}


bool Input::GetInputPad()
{

	if (TRUE)
	{
		if (CheckHitKeyAll(DX_CHECKINPUT_PAD) != 0)
		{
			return TRUE;
		}
		
	}
	else
	{
		for (int i = 0; i < 16; i++)
		{
			if (now_type_state_.pad.Buttons[i] != 0)
			{
				return TRUE;
			}
		}

		if ((now_type_state_.pad.ThumbLX <= kPadStickDeadZone &&
			now_type_state_.pad.ThumbLX >= -kPadStickDeadZone) ||
			(now_type_state_.pad.ThumbLY <= kPadStickDeadZone &&
				now_type_state_.pad.ThumbLY >= -kPadStickDeadZone) ||
			(now_type_state_.pad.ThumbRX <= kPadStickDeadZone &&
				now_type_state_.pad.ThumbRX >= -kPadStickDeadZone) ||
			(now_type_state_.pad.ThumbRY <= kPadStickDeadZone &&
				now_type_state_.pad.ThumbRY >= -kPadStickDeadZone))
		{
			return TRUE;
		}


		if (now_type_state_.pad.LeftTrigger > 0 ||
			now_type_state_.pad.RightTrigger > 0)
		{
			return TRUE;
		}


	}

	
	return FALSE;
}


void Input::DecideDeviceType()
{
	
	bool is_key = GetInputKey();
	bool is_pad = GetInputPad();
	
	//何が入力されているか


	switch(device_type_)
	{
	case InputDeviceType::kNothing:

		//キーボード入力検知
		if (is_key)
		{
			//キーボード入力されたら
			device_type_ = InputDeviceType::kKey;
			return;
		}

		//pad入力検知
		if (is_pad)
		{
			device_type_ = InputDeviceType::kPad;
			return;
		}

		return;

		break;


	case InputDeviceType::kKey:

		//キーボード入力検知
		if (is_key)
		{
			//続けて入力されているなら
			return;
		}

		if (is_pad)
		{
			device_type_ = InputDeviceType::kPad;
			return;
		}
		return;
		
		break;


	case InputDeviceType::kPad:

		if (is_pad)
		{
			return;
		}

		//キーボード入力検知
		if (is_key)
		{
			//続けて入力されているなら
			device_type_ = InputDeviceType::kKey;
			return;
		}

		return;

		break;

	}



}


VECTOR Input::GetVerticalVector(const VECTOR& next_pos, const VECTOR& pos)
{

	// 次の座標 - 今の座標
	return VGet(next_pos.x - pos.x,
		next_pos.y - pos.y,
		next_pos.z - pos.z);


}


float Input::MakePercent(float value, float min, float max)
{
	
	if (value > 0)
	{
		return (value + min) / (max + min);
	}
	else if (value < 0)
	{
		return -((value - min) / (-(max + min)));
	}

}

/*-----------public-----------*/

void Input::Awake(const int num)
{
	num_ = num;
}

void Input::Update()
{
	//パッドのスティックの位置更新は行っておく
	before_type_state_.pad.ThumbLX = now_type_state_.pad.ThumbLX;
	before_type_state_.pad.ThumbLY = now_type_state_.pad.ThumbLY;
	before_type_state_.pad.ThumbRX = now_type_state_.pad.ThumbRX;
	before_type_state_.pad.ThumbRY = now_type_state_.pad.ThumbRY;

	before_type_state_.left_stick_rad = now_type_state_.left_stick_rad;
	before_type_state_.right_stick_rad = now_type_state_.right_stick_rad;

	GetHitKeyStateAll(now_type_state_.key);

	now_type_state_.atai = GetMouseInputLog2(&now_type_state_.mouse, &now_type_state_.mouse_x,
		&now_type_state_.mouse_y, &now_type_state_.log, TRUE);

	GetMousePoint(&now_type_state_.mouse_x, &now_type_state_.mouse_y);
	GetJoypadXInputState(num_,&(now_type_state_.pad));

	DecideDeviceType();
	
}


void Input::SetTypeState(const  InputType& now_input, const InputType& before_input)
{
	now_type_state_ = now_input;
	before_type_state_ = before_input;
}


void Input::ResetMousePoint()
{
	int center_mouse_x = static_cast<int>(kGameWidth * 0.5f);
	int center_mouse_y = static_cast<int>(kGameHeight * 0.5f);
	SetMousePoint(center_mouse_x,center_mouse_y);
}

InputState Input::CheckInputKey(int key_code)
{

	InputState state = InputState::kOff;
		
	//押していない
	if (before_type_state_.key[key_code] == 0 && now_type_state_.key[key_code] == 0) { state = InputState::kOff; }
	//押した瞬間
	if (before_type_state_.key[key_code] == 0 && now_type_state_.key[key_code] == 1) { state = InputState::kPush; }
	//押し続けているとき
	if (before_type_state_.key[key_code] == 1 && now_type_state_.key[key_code] == 1) { state = InputState::kOn; }
	//離した瞬間
	if (before_type_state_.key[key_code] == 1 && now_type_state_.key[key_code] == 0) { state = InputState::kRelease; }

	before_type_state_.key[key_code] = now_type_state_.key[key_code];

	return state;

}

InputState Input::CheckInputMouse(int mouse)
{
	InputState state = InputState::kOff;
	
	//押したのと押していない状態を取る

	if (now_type_state_.atai == 0)
	{
		if ((now_type_state_.mouse & mouse) && (now_type_state_.log == MOUSE_INPUT_LOG_DOWN))
		{
			state = InputState::kPush;
		}
		if ((now_type_state_.mouse & mouse) && (now_type_state_.log == MOUSE_INPUT_LOG_UP))
		{
			state = InputState::kRelease;
		}
		before_type_state_.input_state = state;
		
	}
	

	if(now_type_state_.atai == -1)
	{

		//前回の入力を見て
		//前回がpushなら
		
		if (before_type_state_.input_state == InputState::kPush)
		{
			state = InputState::kOn;
		}

		if (before_type_state_.input_state == InputState::kRelease)
		{
			state = InputState::kOff;
		}

	}

	//printfDx("%d\n", now_type_state_.atai);

	return state;


}

InputState Input::CheckInputPadButton(int pad_button)
{
	InputState state = InputState::kOff;

	// 押していない
	if ((before_type_state_.pad.Buttons[pad_button]) == 0 &&
		(now_type_state_.pad.Buttons[pad_button]) == 0)
	{ state = InputState::kOff; }
	// 押した瞬間
	if ((before_type_state_.pad.Buttons[pad_button]) == 0 &&
		(now_type_state_.pad.Buttons[pad_button]) == 1)
	{ state = InputState::kPush; }
	// 押し続けているとき
	if ((before_type_state_.pad.Buttons[pad_button]) == 1 &&
		(now_type_state_.pad.Buttons[pad_button]) == 1)
	{ state = InputState::kOn; }
	// 離した瞬間
	if ((before_type_state_.pad.Buttons[pad_button]) == 1 &&
		(now_type_state_.pad.Buttons[pad_button]) == 0)
	{ state = InputState::kRelease; }

	before_type_state_.pad.Buttons[pad_button] =
		now_type_state_.pad.Buttons[pad_button];

	return state;
}


float Input::GetMouseVertical()
{
	float vertical_num = 0.0f;

	//画面の中心を調べる
	VECTOR center_pos = VGet((kGameWidth * 0.5f), (kGameHeight * 0.5f), 0.0f);

	//2点間の大きさのvector
	VECTOR vertical_vec = VGet((now_type_state_.mouse_x - center_pos.x),
		(now_type_state_.mouse_y - center_pos.y), 0.0f);

	vertical_num = sqrt(TheNumPower(vertical_vec.x, 2) + 
		TheNumPower(vertical_vec.y, 2));

	return vertical_num;

}


float Input::GetMouseRad()
{
	VECTOR rad_vec = VGet(0.0f, 0.0f, 0.0f);
	float rad = 0.0f;

	rad_vec = GetVerticalVector(VGet((now_type_state_.mouse_x), (now_type_state_.mouse_y), 0.0f),
		VGet((kGameWidth * 0.5f), (kGameHeight * 0.5f), 0.0f));

	rad = atan2f(rad_vec.x, rad_vec.y);

	return rad;
}


float Input::GetPadStickVertical(int type)
{
	int vertical_num = 0.0f;

	if (type == StickType::kLeft)
	{
		//三平方
		vertical_num = sqrt(TheNumPower((now_type_state_.pad.ThumbLX * 0.01f), 2) +
			(TheNumPower((now_type_state_.pad.ThumbLY * 0.01f), 2)));
		//printfDx("%d\n", vertical_num);

	}
	else if(type == StickType::kRight)
	{
		//三平方
		vertical_num = sqrt(TheNumPower((now_type_state_.pad.ThumbRX * 0.01f), 2) +
			(TheNumPower((now_type_state_.pad.ThumbRY * 0.01f), 2)));
	}

	
	//printfDx("%d\n", vertical_num);
	return static_cast<float>(vertical_num);
}


float Input::GetPadStickRad(int type)
{
	// 処理はbrainのターゲットしたときと一緒

	VECTOR rad_vec = VGet(0.0f, 0.0f, 0.0f);
	float rad = 0.0f;

	if (type == StickType::kLeft)
	{
		rad_vec = GetVerticalVector(VGet(now_type_state_.pad.ThumbLX, now_type_state_.pad.ThumbLY, 0.0f),
			VGet(0.0f, 0.0f, 0.0f));

		rad = atan2f(rad_vec.x, rad_vec.y);
		now_type_state_.left_stick_rad = rad;
	}
	else if(type == StickType::kRight)
	{
		rad_vec = GetVerticalVector(VGet(now_type_state_.pad.ThumbRX, now_type_state_.pad.ThumbRY, 0.0f),
			VGet(0.0f, 0.0f, 0.0f));

		rad = atan2f(rad_vec.x, rad_vec.y);
		now_type_state_.right_stick_rad = rad;

	}
	else
	{
		printfDx("error");
		return rad;
	}


	return rad;
}


float Input::GetPadStickPercent(int type, int control)
{
	float percent_num = 0.0f;
	
	

	//スティックの左右
	if (type == StickType::kLeft)
	{
		if (control == Control::kX)
		{

			if (-kPadStickDeadZone < now_type_state_.pad.ThumbLX && now_type_state_.pad.ThumbLX < kPadStickDeadZone)
			{
				return percent_num;
			}

			percent_num = MakePercent(now_type_state_.pad.ThumbLX, kPadStickDeadZone, kMaxPadStickNum);
		}
		else if(control == Control::kY)
		{

			if (-kPadStickDeadZone < now_type_state_.pad.ThumbLY && now_type_state_.pad.ThumbLY < kPadStickDeadZone)
			{
				return percent_num;
			}

			percent_num = MakePercent(now_type_state_.pad.ThumbLY, kPadStickDeadZone, kMaxPadStickNum);
		}
		else
		{
			printfDx("error");
		}

	}
	else if (type == StickType::kRight)
	{
		if (control == Control::kX)
		{
			if (-kPadStickDeadZone < now_type_state_.pad.ThumbRX && now_type_state_.pad.ThumbRX < kPadStickDeadZone)
			{
				return percent_num;
			}

			percent_num = MakePercent(now_type_state_.pad.ThumbRX, kPadStickDeadZone, kMaxPadStickNum);
		}
		else if (control == Control::kY)
		{

			if (-kPadStickDeadZone < now_type_state_.pad.ThumbRY && now_type_state_.pad.ThumbRY < kPadStickDeadZone)
			{
				return percent_num;
			}

			percent_num = MakePercent(now_type_state_.pad.ThumbRY, kPadStickDeadZone, kMaxPadStickNum);
		}
		else
		{
			printfDx("error");
		}

	}
	else
	{
		printfDx("error");
	}

	return percent_num;
}


float Input::GetMousePercent(int control)
{
	float percent_num = 0.0f;

	//中心からの距離
	float center_to_mouse_x = now_type_state_.mouse_x - (kGameWidth * 0.5f);
	float center_to_mouse_y = now_type_state_.mouse_y - (kGameHeight * 0.5f);

	/*画面外に行ったときの処理*/
	if (true)
	{
		if (center_to_mouse_x < -(kGameWidth * 0.5f)) { center_to_mouse_x = -(kGameWidth * 0.5f); }		//左
		if (center_to_mouse_x > (kGameWidth * 0.5f)) { center_to_mouse_x = (kGameWidth * 0.5f); }			//右
		if (center_to_mouse_y < -(kGameHeight * 0.5f)) { center_to_mouse_y = -(kGameHeight * 0.5f); }		//上
		if (center_to_mouse_y > (kGameHeight * 0.5f)) { center_to_mouse_y = (kGameHeight * 0.5f); }		//下
	}
	

	//x座標が選択されている
	if (control == Control::kX)
	{
		if (-kMouseDeadZone < center_to_mouse_x && center_to_mouse_x < kMouseDeadZone)
		{
			return percent_num;
		}
		percent_num = MakePercent(center_to_mouse_x, kMouseDeadZone, (kGameWidth * 0.5f));
	}
	else if(control == Control::kY)
	{
		if (-kMouseDeadZone < center_to_mouse_y && center_to_mouse_y < kMouseDeadZone)
		{
			return percent_num;
		}
		
		percent_num = MakePercent(center_to_mouse_y, kMouseDeadZone, (kGameHeight * 0.5f));

	}
	else
	{
		printfDx("error");
	}

	return percent_num;
}

float Input::GetStickSpin(int type)
{

	/*
	//余弦定理によってcosθを取得し、acosfによって角度を求める

	//二辺のVECTORを用意する
	VECTOR center_to_before_velocity = VGet(0.f, 0.f, 0.f);		//中心からbeforeの距離を求める変数
	VECTOR center_to_now_velocity    = VGet(0.f, 0.f, 0.f);		//中心からnowの距離を求める変数

	if (type == StickType::kLeft)
	{
		//左スティックの回転量を取得
		center_to_before_velocity.x = before_type_state_.pad.ThumbLX;
		center_to_before_velocity.y = before_type_state_.pad.ThumbLY;

		center_to_now_velocity.x    = now_type_state_.pad.ThumbLX;
		center_to_now_velocity.y    = now_type_state_.pad.ThumbLY;
	}
	else if (type == StickType::kRight)
	{
		//右スティックの回転量を取得
		center_to_before_velocity.x = before_type_state_.pad.ThumbRX;
		center_to_before_velocity.y = before_type_state_.pad.ThumbRY;

		center_to_now_velocity.x    = now_type_state_.pad.ThumbRX;
		center_to_now_velocity.y    = now_type_state_.pad.ThumbRY;
	}
	else
	{
		printfDx("error\n");
	}


	//三角形を作ります
	//今2辺作れている

	//θの対になる辺のVECTORを用意
	// VSubで用意する
	VECTOR opposite = VSub(center_to_now_velocity, center_to_before_velocity);

	//3辺のサイズを取得
	float center_to_before_size		= VSize(center_to_before_velocity);
	float center_to_now_size		= VSize(center_to_now_velocity);
	float opposite_size				= VSize(opposite);

	//ここから余弦定理の出番です
	//余弦定理
	//cosC = a^2 + b^2 - c^2 / 2 * a * b

	//余弦定理の結果を入れておく
	float cos_num = (center_to_before_size * center_to_before_size) + (center_to_now_size * center_to_now_size)
		- (opposite_size * opposite_size) / 2 * center_to_before_size * center_to_now_size;

	//アークコサインにradの値を返してもらう

	*/
	//float now_rad = GetPadStickRad(StickType::kRight);
	if (type == StickType::kRight)
	{
		
	}
	else if(type == StickType::kLeft)
	{
		
	}
	else
	{
		printfDx("error\n");
	}

	

	

	return (GetPadStickRad(StickType::kRight) - before_type_state_.right_stick_rad);
}


void Input::Debug()
{
	DrawFormatString(0, Debug::GetInstance().GetCurrentNum() * Debug::GetInstance().GetFontSize(), GetColor(0, 0, 0), "-----Input-----");
	Debug::GetInstance().Add();

	DrawFormatString(0, Debug::GetInstance().GetCurrentNum() * Debug::GetInstance().GetFontSize(), GetColor(0, 0, 0), "input:");

	switch (device_type_)
	{

	case InputDeviceType::kKey:

		DrawFormatString(Debug::GetInstance().GetFontSize() * 3, Debug::GetInstance().GetCurrentNum() * Debug::GetInstance().GetFontSize(), GetColor(0, 0, 0), "Key");

		break;

	case InputDeviceType::kPad:

		DrawFormatString(Debug::GetInstance().GetFontSize() * 3, Debug::GetInstance().GetCurrentNum() * Debug::GetInstance().GetFontSize(), GetColor(0, 0, 0), "Pad");

		break;

	}

	Debug::GetInstance().Add();

}


void Input::Draw()
{
	int Color = GetColor(255, 255, 255);
	DrawFormatString(0, 0, Color, "LeftTrigger:%d RightTrigger:%d",
		now_type_state_.pad.LeftTrigger, now_type_state_.pad.RightTrigger);
	DrawFormatString(0, 16, Color, "ThumbLX:%d ThumbLY:%d",
		now_type_state_.pad.ThumbLX, now_type_state_.pad.ThumbLY);
	DrawFormatString(0, 32, Color, "ThumbRX:%d ThumbRY:%d",
		now_type_state_.pad.ThumbRX, now_type_state_.pad.ThumbRY);
}
