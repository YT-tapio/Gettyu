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
    static const int kSuperAttackKey = MOUSE_INPUT_RIGHT;
    //static const int kSwordSlash;

    //ìÆÇ´Ç…ÇÕä÷åWÇµÇ»Ç¢Ç‡ÇÃ
    static const int kSwitchWeaponKey = KEY_INPUT_R;
    static const int kSwicthBatKey = KEY_INPUT_1;
    static const int kSwicthWarpRodKey = KEY_INPUT_2;
};

struct PadConfig
{
    static const int kLeftStick = 14000;
    static const int kRightStick = -19000;
    static const int kUpStick = 0;
    static const int kDownStick = 0;
    static const int kJumpButton = XINPUT_BUTTON_A;
    static const int kAttackButton = XINPUT_BUTTON_LEFT_SHOULDER;
    //ìÆÇ´Ç…ÇÕä÷åWÇµÇ»Ç¢Ç‡ÇÃ
    static const int kSuperAttackButton = XINPUT_BUTTON_RIGHT_SHOULDER;
    static const int kSwitchWarpRodButton = XINPUT_BUTTON_Y;
    static const int kSwitchBatButton = XINPUT_BUTTON_X;
    static const int kSwitchDashHoopButton = XINPUT_BUTTON_B;
};
