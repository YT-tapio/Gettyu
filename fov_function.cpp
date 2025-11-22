#include"fov_function.h"

bool IsInFov(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const float& fov)
{
	float dot = GetDotRad(pos1, pos2, pos3);
	float herf_fov = fov * 0.5f;

	//fov‚È‚¢‚È‚ç
	if (dot <= herf_fov)
	{
		return TRUE;
	}

	//”ÍˆÍ“à‚Å‚Í‚È‚¢‚Æ‚«‚ÍFALSE
	return FALSE;
}

float GetDotRad(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3)
{

	VECTOR n_pos1_to_pos2 = VNorm(VSub(pos2, pos1));		//enemy‚©‚çplayer
	VECTOR n_pos1_to_pos3 = VNorm(VSub(pos3, pos1));		//enemy‚©‚ç‚³‚«‚Ù‚Ç‚«‚ß‚½target(way_point)

	//dot‚Å‹‚ß‚é
	float dot = VDot(n_pos1_to_pos2, n_pos1_to_pos3);

	return dot;

}