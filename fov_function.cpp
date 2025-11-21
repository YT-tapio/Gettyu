#include"fov_function.h"

bool IsInFov(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const float& fov)
{
	VECTOR n_pos1_to_pos2 = VNorm(VSub(pos2, pos1));		//enemyÇ©ÇÁplayer
	VECTOR n_pos1_to_pos3 = VNorm(VSub(pos3, pos1));		//enemyÇ©ÇÁÇ≥Ç´ÇŸÇ«Ç´ÇﬂÇΩtarget(way_point)

	//dotÇ≈ãÅÇﬂÇÈ
	float dot = VDot(n_pos1_to_pos2, n_pos1_to_pos3);
	float herf_fov = fov * 0.5f;

	//fovÇ»Ç¢Ç»ÇÁ
	if (dot <= herf_fov)
	{
		return TRUE;
	}

	//îÕàÕì‡Ç≈ÇÕÇ»Ç¢Ç∆Ç´ÇÕFALSE
	return FALSE;
}