#include"DxLib.h"
#include<math.h>
#include"input.h"

Input::Input(int num)
	: num_(num)
{

}


Input::~Input()
{
	
}

/*-----------private----------*/

VECTOR Input::GetVerticalVector(const VECTOR& next_pos, const VECTOR& pos)
{

	// Ÿ‚ÌÀ•W - ¡‚ÌÀ•W
	return VGet(next_pos.x - pos.x,
		next_pos.x - pos.x,
		next_pos.x - pos.x);


}


/*-----------public-----------*/

void Input::Update()
{
	GetHitKeyStateAll(now_type_state_.key);
	now_type_state_.mouse = GetMouseInput();
	GetMousePoint(&now_type_state_.mouse_x, &now_type_state_.mouse_y);
	GetJoypadXInputState(num_,&(now_type_state_.pad));
}


InputState Input::CheckInputKey(int key_code)
{

	InputState state;

	//‰Ÿ‚µ‚Ä‚¢‚È‚¢
	if (before_type_state_.key[key_code] == 0 && now_type_state_.key[key_code] == 0) { state = InputState::kOff; }
	//‰Ÿ‚µ‚½uŠÔ
	if (before_type_state_.key[key_code] == 0 && now_type_state_.key[key_code] == 1) { state = InputState::kPush; }
	//‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚Æ‚«
	if (before_type_state_.key[key_code] == 1 && now_type_state_.key[key_code] == 1) { state = InputState::kOn; }
	//—£‚µ‚½uŠÔ
	if (before_type_state_.key[key_code] == 1 && now_type_state_.key[key_code] == 0) { state = InputState::kRelease; }

	before_type_state_.key[key_code] = now_type_state_.key[key_code];

	return state;

}

InputState Input::CheckInputMouse(int mouse)
{
	InputState state;

	// ‰Ÿ‚µ‚Ä‚¢‚È‚¢
	if ((before_type_state_.mouse & mouse) == 0 && (now_type_state_.mouse & mouse) == 0) { state = InputState::kOff; }
	// ‰Ÿ‚µ‚½uŠÔ
	if ((before_type_state_.mouse & mouse) == 0 && (now_type_state_.mouse & mouse) == 1) { state = InputState::kPush; }
	// ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚Æ‚«
	if ((before_type_state_.mouse & mouse) == 1 && (now_type_state_.mouse & mouse) == 1) { state = InputState::kOn; }
	// —£‚µ‚½uŠÔ
	if ((before_type_state_.mouse & mouse) == 1 && (now_type_state_.mouse & mouse) == 0) { state = InputState::kRelease; }

	before_type_state_.mouse = now_type_state_.mouse;

	return state;


}

InputState Input::CheckInputPadButton(int pad_button)
{
	InputState state;

	// ‰Ÿ‚µ‚Ä‚¢‚È‚¢
	if ((before_type_state_.pad.Buttons[pad_button]) == 0 &&
		(now_type_state_.pad.Buttons[pad_button]) == 0)
	{ state = InputState::kOff; }
	// ‰Ÿ‚µ‚½uŠÔ
	if ((before_type_state_.pad.Buttons[pad_button]) == 0 &&
		(now_type_state_.pad.Buttons[pad_button]) == 1)
	{ state = InputState::kPush; }
	// ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚Æ‚«
	if ((before_type_state_.pad.Buttons[pad_button]) == 1 &&
		(now_type_state_.pad.Buttons[pad_button]) == 1)
	{ state = InputState::kOn; }
	// —£‚µ‚½uŠÔ
	if ((before_type_state_.pad.Buttons[pad_button]) == 1 &&
		(now_type_state_.pad.Buttons[pad_button]) == 0)
	{ state = InputState::kRelease; }

	before_type_state_.pad.Buttons[pad_button] =
		now_type_state_.pad.Buttons[pad_button];

	return state;
}

float Input::GetPadStickVertical()
{
	float vertical_num = 0.0f;

	//O•½•û
	vertical_num = sqrt((now_type_state_.pad.ThumbLX * now_type_state_.pad.ThumbLX) +
		((now_type_state_.pad.ThumbLY * now_type_state_.pad.ThumbLY)));


	return vertical_num;
}


float Input::GetPadStickRad()
{
	// ˆ—‚Íbrain‚Ìƒ^[ƒQƒbƒg‚µ‚½‚Æ‚«‚Æˆê

	VECTOR rad_vec = VGet(0.0f, 0.0f, 0.0f);
	float rad = 0.0f;

	//“ü—Í‚µ‚½êŠ‚©‚ç’†S‚ÌêŠ‚ğˆø‚­
	rad_vec = GetVerticalVector(VGet(now_type_state_.pad.ThumbLX, now_type_state_.pad.ThumbLY, 0.0f),
		VGet(0.0f, 0.0f, 0.0f));

	rad = atan2f(rad_vec.x,rad_vec.y);

	return rad;

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





