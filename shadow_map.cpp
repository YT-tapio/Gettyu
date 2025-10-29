#include "shadow_map.h"

ShadowMap::ShadowMap() 
	: size_x_(0)
	, size_y_(0)
	, min_offset_pos_(VGet(0.f, 0.f, 0.f))
	, max_offset_pos_(VGet(0.f, 0.f, 0.f))
	, shadow_map_handle_(MakeShadowMap(kSizeX, kSizeY))
{
	SetShadowMapLightDirection(shadow_map_handle_, VNorm(VGet(0.0f, -1.f, 0.0f)));
}

ShadowMap::~ShadowMap()
{
	DeleteShadowMap(shadow_map_handle_);
}

void ShadowMap::SetupDrawShadowMap()
{
	const auto center_pos = GetCameraPosition();
	const auto min_pos = VAdd(center_pos, min_offset_pos_);
	const auto max_pos = VAdd(center_pos, max_offset_pos_);

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(shadow_map_handle_, min_pos, max_pos);

	ShadowMap_DrawSetup(shadow_map_handle_);
}

void ShadowMap::EndDrawShadowMap()
{
	ShadowMap_DrawEnd();
}

void ShadowMap::UseShadowMap()
{
	SetUseShadowMap(0, shadow_map_handle_);
}

void ShadowMap::UnuseShadowMap()
{
	SetUseShadowMap(0, -1);
}