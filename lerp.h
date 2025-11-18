#pragma once
#include"Dxlib.h"

/// <summary>
/// ê¸å`ï€ä«Ç∑ÇÈÇ‚Å[Ç¬
/// </summary>
/// <param name="now_pos"></param>
/// <param name="target_pos"></param>
/// <param name="time"></param>
/// <param name="timer"></param>
/// <param name="flag"></param>
/// <returns></returns>
VECTOR Lerp(const VECTOR& start_pos,const VECTOR& now_pos, const VECTOR& target_pos, const float max_time, float& timer, bool& flag);