#pragma once
#include"Dxlib.h"

class ShadowMap
{
private:

	static constexpr int	kSizeX			= 8192;
	static constexpr int	kSizeY			= 8192;
	static constexpr VECTOR kMinOffsetPos	= { -700.0f, -10.0f, -700.0f };
	static constexpr VECTOR kMaxOffsetPos	= { 700.0f, 300.0f,  700.0f };

	int		size_x_;
	int		size_y_;
	VECTOR	min_offset_pos_;
	VECTOR	max_offset_pos_;

	int shadow_map_handle_;

public:

	ShadowMap();
	~ShadowMap();

	void SetupDrawShadowMap();
	void EndDrawShadowMap();

	void UseShadowMap();
	void UnuseShadowMap();

};