#pragma once
#include"DxLib.h"

/*-------ëÄçÏèåè-------*/

struct KeyConfig
{
    static const int kUpKey = KEY_INPUT_W;
    static const int kDownKey = KEY_INPUT_S;
    static const int kLeftKey = KEY_INPUT_A;
    static const int kRightKey = KEY_INPUT_D;
    static const int kJumpKey = KEY_INPUT_SPACE;
};

struct PadConfig
{
    static const int kLeftStick = 14000;
    static const int kRightStick = -19000;
    static const int kUpStick = 0;
    static const int kDownStick = 0;
    static const int kJumpButton = XINPUT_BUTTON_A;
};
