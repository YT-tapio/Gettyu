#pragma once
#include"DxLib.h"

struct UIGraphData
{
	VECTOR pos;					// ポジション
	int handle;						// 画像データ

	float original_width;		// 元の画像の大きさ(横)
	float original_height;		// 元の画像の大きさ(縦)

	float width;					// ちょうせいしたいサイズ(横)
	float height;					// ちょうせいしたいサイズ(縦)
};

void OffsetGraphSize(UIGraphData& graph_data);

void DrawUIGraph(const UIGraphData& graph_data);
