#pragma once
#include"Dxlib.h"

/// <summary>
/// 線形保管するやーつ(時間)
/// </summary>
/// <param name="now_pos"></param>
/// <param name="target_pos"></param>
/// <param name="time"></param>
/// <param name="timer"></param>
/// <param name="flag"></param>
/// <returns></returns>
VECTOR TimeLerp(const VECTOR& start_pos,const VECTOR& now_pos, const VECTOR& target_pos, const float max_time, float& timer, bool& flag);

/// <summary>
/// 線形保管するやーつ(これを線形補完と呼んでいいのだろうか)
/// </summary>
/// <param name="now_pos">いまのpos</param>
/// <param name="target_pos">行きたい場所</param>
/// <param name="speed">delta_timeをかけたspeedを持ち込ませる</param>
/// <param name="flag"></param>
/// <returns></returns>
VECTOR NormalLerp(const VECTOR& now_pos, const VECTOR& target_pos, const float& speed, bool& flag);

