#pragma once
#include"DxLib.h"

/// @brief ‚±‚Ìfov“à‚É‚¢‚é‚Ì‚©‚Ç‚¤‚©‚Ì”»’f‚ğs‚¤
/// @param pos1 ‹N“_‚Æ‚È‚épos
/// @param pos2 ³–Ê
/// @param pos3 ’²‚×‚½‚¢êŠ
/// @param fov ‹–ìŠp
/// @return ’†‚É‚¢‚é‚©‚Ç‚¤‚©
bool IsInFov(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const float& fov);
