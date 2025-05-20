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
    static const int kJumpButton = XINPUT_BUTTON_A;
    static const int kAttackButton = XINPUT_BUTTON_X;
    static const int kLeftButton = 14000;
    static const int kRightButton = -19000;
};
