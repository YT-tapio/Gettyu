#pragma once
#include"DxLib.h"
#include"mask.h"

struct UIGraphData
{
	VECTOR pos;					// ポジション
	int handle;						// 画像データ

	float original_width;		// 元の画像の大きさ(横)
	float original_height;		// 元の画像の大きさ(縦)

	float width;					// ちょうせいしたいサイズ(横)
	float height;					// ちょうせいしたいサイズ(縦)

	float width_ratio;
	float height_ratio;

};


struct UI3DModelData
{
	MATRIX mat;
	VECTOR pos;
	VECTOR rot;
	VECTOR scale;

	int handle;


};

;

void OffsetGraphSize(UIGraphData& graph_data);

void DrawUIGraph(const UIGraphData& graph_data);

void DrawMaskBox(const MaskData& mask_data);

void Set3DModelMatrix(UI3DModelData& data);

void Draw3DModel(const UI3DModelData& data);

void SizeUp(UIGraphData& data,bool& flag, int width, int height, float speed,int count);

void SizeDown(UIGraphData& data, bool& flag, int width, int height, float speed, int count);

//上下に揺らす処理
VECTOR UpDown(const VECTOR& init_pos, float& rad, float speed, float swing);