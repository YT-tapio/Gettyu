#include<math.h>

#include"UI_data.h"
#include"FPS.h"
#include"const_rad.h"

void OffsetGraphSize(UIGraphData& graph_data)
{
	//ratioを保存
	graph_data.width_ratio = graph_data.original_width / (graph_data.original_width + graph_data.original_height);
	graph_data.height_ratio = graph_data.original_height / (graph_data.original_width + graph_data.original_height);

	//もともとwidth,heightに入れておく
	graph_data.width = graph_data.width * (graph_data.width_ratio);
	graph_data.height = graph_data.height * (graph_data.height_ratio);
}

void DrawUIGraph(const UIGraphData& graph_data)
{
	DrawExtendGraph(static_cast<int>(graph_data.pos.x - (graph_data.width * 0.5f)), static_cast<int>(graph_data.pos.y - (graph_data.height * 0.5f))
		, static_cast<int>(graph_data.pos.x + (graph_data.width * 0.5f)), static_cast<int>(graph_data.pos.y + (graph_data.height * 0.5f)), graph_data.handle, TRUE);
}

void DrawMaskBox(const MaskData& data)
{
	DrawBox(static_cast<int>(data.pos.x), static_cast<int>(data.pos.y),
		static_cast<int>(data.pos.x + data.width), static_cast<int>(data.pos.y + data.height), data.color, TRUE);
}

void Set3DModelMatrix(UI3DModelData& data)
{
	MATRIX rot_mat = MMult(MMult(MGetRotX(data.rot.x), MGetRotY(data.rot.y)), MGetRotZ(data.rot.z));
	MATRIX scale_mat = MGetScale(data.scale);
	MATRIX pos_mat = MGetTranslate(data.pos);
	data.mat = MMult(MMult(scale_mat, rot_mat), pos_mat);

	MV1SetMatrix(data.handle, data.mat);
}

void Draw3DModel(const UI3DModelData& data)
{
	MV1DrawModel(data.handle);
}

void SizeUp(UIGraphData& data,bool& flag,int width, int height,float speed,int count)
{

	auto delta_time = FPS::GetInstance().GetDeltaTime();

	bool width_dazed = FALSE;
	bool height_dazed = FALSE;

	//比を生成
	float diff_width = width - data.width;
	float diff_height = height - data.height;

	float all_diff_size = diff_width + diff_height;
	float width_ratio = diff_width / all_diff_size;
	float height_ratio = diff_height / all_diff_size;



	//大きくする
	data.width = data.width + (speed * width_ratio * count * delta_time);
	data.height = data.height + (speed* height_ratio * count * delta_time);

	
	if (data.width >= width)
	{
		data.width = width;
		width_dazed = TRUE;
	}

	if (data.height >= height)
	{
		data.height = height;
		height_dazed = TRUE;
	}

	if (width_dazed && height_dazed)
	{
		flag = FALSE;
		return;
	}
}

void SizeDown(UIGraphData& data, bool& flag, int width, int height, float speed, int count)
{
	auto delta_time = FPS::GetInstance().GetDeltaTime();

	bool width_dazed = FALSE;
	bool height_dazed = FALSE;

	//比を生成
	//マイナスになる
	float diff_width = width - data.width;
	float diff_height = height - data.height;

	float all_diff_size = fabs(diff_width + diff_height);
	float width_ratio = diff_width / all_diff_size;
	float height_ratio = diff_height / all_diff_size;



	//小さくする
	data.width = data.width + (speed * width_ratio * count * delta_time);
	data.height = data.height + (speed * height_ratio * count * delta_time);


	if (data.width <= width)
	{
		data.width = width;
		width_dazed = TRUE;
	}

	if (data.height <= height)
	{
		data.height = height;
		height_dazed = TRUE;
	}

	if (width_dazed && height_dazed)
	{
		flag = FALSE;
		return;
	}
}


VECTOR UpDown(const VECTOR& init_pos, float& rad, float speed, float swing)
{
	VECTOR next_pos = init_pos;

	//sinを使って上下に動かす

	rad += (kOneRad * (speed * FPS::GetInstance().GetDeltaTime()));


	//180を超えるようなら
	if (rad > kReverceRad)
	{
		//-180から180の間に強制変換
		rad -= (kReverceRad + kReverceRad);
	}


	next_pos = VAdd(next_pos, VGet(0.f, (swing * sinf(rad)), 0.f));

	return next_pos;
}