#pragma once
#include"DxLib.h"

/*-------ëÄçÏèåè-------*/

struct KeyConfig
{
    static const int kUpKey = KEY_INPUT_W;
    static const int kDownKey = KEY_INPUT_S;
    static const int kLeftKey = KEY_INPUT_A;
    static const int kRightKey = KEY_INPUT_D;
    static const int kDashKey = KEY_INPUT_LSHIFT;
    static const int kWalkKey = KEY_INPUT_LCONTROL;
    static const int kJumpKey = KEY_INPUT_SPACE;
    static const int kAttackKey = MOUSE_INPUT_LEFT;
    //static const int kSwordSlash;

    //ìÆÇ´Ç…ÇÕä÷åWÇµÇ»Ç¢Ç‡ÇÃ
    static const int kSwitchWeaponKey = KEY_INPUT_R;

};

struct PadConfig
{
    static const int kLeftStick = 14000;
    static const int kRightStick = -19000;
    static const int kUpStick = 0;
    static const int kDownStick = 0;
    static const int kJumpButton = XINPUT_BUTTON_A;
    static const int kAttackButton = XINPUT_BUTTON_X;

    //ìÆÇ´Ç…ÇÕä÷åWÇµÇ»Ç¢Ç‡ÇÃ
    static const int kSwitchWeaponButton = XINPUT_BUTTON_RIGHT_SHOULDER;

};
