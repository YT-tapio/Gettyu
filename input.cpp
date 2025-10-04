#include"DxLib.h"
#include<math.h>
#include"screen.h"
#include"Calculation.h"
#include"input.h"


Input::Input(const int num)
	: num_(num)
{

}


Input::~Input()
{
	
}

/*-----------private----------*/

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

void Input::Update()
{
	GetHitKeyStateAll(now_type_state_.key);

	now_type_state_.atai = GetMouseInputLog2(&now_type_state_.mouse, &now_type_state_.mouse_x,
		&now_type_state_.mouse_y, &now_type_state_.log, TRUE);

	GetMousePoint(&now_type_state_.mouse_x, &now_type_state_.mouse_y);
	GetJoypadXInputState(num_,&(now_type_state_.pad));
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

	}
	else if(type == StickType::kRight)
	{
		rad_vec = GetVerticalVector(VGet(now_type_state_.pad.ThumbRX, now_type_state_.pad.ThumbRY, 0.0f),
			VGet(0.0f, 0.0f, 0.0f));

		rad = atan2f(rad_vec.x, rad_vec.y);
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

/*
float Input::GetStickSpinRad(float move_rad,const int type)
{
	//nowとbeforeを比べる
	//比べて何度分回転しているのかを調べてそれを返す
	float rad = 0.f;
	//まえのradと今回のradを比べる
	//回転量をとりあえず調べよう

	if (type == StickType::kLeft)
	{
		rad = before_type_state_.left_stick_rad - now_type_state_.left_stick_rad;
	}
	else if(type == StickType::kRight)
	{
		rad = before_type_state_.right_stick_rad - now_type_state_.right_stick_rad;
	}

	//最低の移動量を越えなければ強制的に0
	if (fabs(rad) <= move_rad)
	{
		rad = 0.0f;
	}

	return rad;

}
*/



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
