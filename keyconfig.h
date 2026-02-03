#pragma once
#include"DxLib.h"

const int kSelectSize = 3;

const int kSelectUpSize      = 2;
const int kSelectDownSize    = 2;
const int kSelectRightSize   = 2;
const int kSelectLeftSize    = 2;
/*-------操作条件-------*/

struct KeyConfig
{
    //デバックボタン
    static const int kChageDebugKey         = KEY_INPUT_N;

    //シーン遷移ボタンのようななもの
    static const int kChangeSceneKey        = KEY_INPUT_SPACE;
    static const int kGameToResultKey       = KEY_INPUT_TAB;

    static const int kUpKey                 = KEY_INPUT_W;
    static const int kDownKey               = KEY_INPUT_S;
    static const int kLeftKey               = KEY_INPUT_A;
    static const int kRightKey              = KEY_INPUT_D;
    static const int kDashKey               = KEY_INPUT_LSHIFT;
    static const int kWalkKey               = KEY_INPUT_LCONTROL;
    static const int kJumpKey               = KEY_INPUT_SPACE;
    static const int kAttackKey             = MOUSE_INPUT_LEFT;
    static const int kSuperAttackKey        = MOUSE_INPUT_RIGHT;
    
    //static const int kSwordSlash;

    //動きには関係しないもの
    static const int kSwitchWeaponKey       = KEY_INPUT_R;
    static const int kSwicthBatKey          = KEY_INPUT_2;
    static const int kSwicthWarpRodKey      = KEY_INPUT_1;

    // 選択ボタン
    static constexpr int kSelectKey[kSelectSize]    = { KEY_INPUT_RETURN ,KEY_INPUT_SPACE,KEY_INPUT_F};
    static const int kSelectMouseButton             = MOUSE_INPUT_1;

    static constexpr int kSelectUpKey[kSelectUpSize]        = { KEY_INPUT_UP,KEY_INPUT_W };
    static constexpr int kSelectDownKey[kSelectDownSize]    = { KEY_INPUT_DOWN,KEY_INPUT_S };
    static constexpr int kSelectRightKey[kSelectRightSize]  = { KEY_INPUT_RIGHT,KEY_INPUT_D };
    static constexpr int kSelectLeftKey[kSelectLeftSize]    = { KEY_INPUT_LEFT,KEY_INPUT_A };
};

struct PadConfig
{
    //デバックボタン
    static const int kChageDebugButton      = XINPUT_BUTTON_BACK;


    //シーン遷移ボタンのようなもの
    static const int kChangeSceneButton     = XINPUT_BUTTON_A;
    static const int kGameToResultButton    = XINPUT_BUTTON_START;

    static const int kLeftStick             = 14000;
    static const int kRightStick            = -19000;
    static const int kUpStick               = 0;
    static const int kDownStick             = 0;
    static const int kJumpButton            = XINPUT_BUTTON_A;
    static const int kAttackButton          = XINPUT_BUTTON_LEFT_SHOULDER;
    static const int kCameraMoveButtonValue = 100;

    //動きには関係しないもの
    static const int kSuperAttackButton     = XINPUT_BUTTON_RIGHT_SHOULDER;
    static const int kSwitchWarpRodButton   = XINPUT_BUTTON_Y;
    static const int kSwitchBatButton       = XINPUT_BUTTON_X;
    static const int kSwitchDashHoopButton  = XINPUT_BUTTON_B;

    // 選択ボタン
    static const int kSelectButton          = XINPUT_BUTTON_A;
    static const int kSelectCancelButton    = XINPUT_BUTTON_B;

    static const int kSelectUpButton        = XINPUT_BUTTON_DPAD_UP;
    static const int kSelectDownButton      = XINPUT_BUTTON_DPAD_DOWN;
    static const int kSelectRightButton     = XINPUT_BUTTON_DPAD_RIGHT;
    static const int kSelectLeftButton      = XINPUT_BUTTON_DPAD_LEFT;

    static const int kSelectUpStick         = 10000;
    static const int kSelectDownStick       = -10000;
    static const int kSelectLeftStick       = -10000;
    static const int kSelectRightStick      = 10000;
};
