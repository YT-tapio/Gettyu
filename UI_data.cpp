#include"UI_data.h"

void OffsetGraphSize(UIGraphData& graph_data)
{
	//‚à‚Æ‚à‚Æwidth,height‚É“ü‚ê‚Ä‚¨‚­
	graph_data.width = graph_data.width * (graph_data.original_width / (graph_data.original_width + graph_data.original_height));
	graph_data.height = graph_data.height * (graph_data.original_height / (graph_data.original_width + graph_data.original_height));
}

void DrawUIGraph(const UIGraphData& graph_data)
{
	DrawExtendGraph(static_cast<int>(graph_data.pos.x - graph_data.width), static_cast<int>(graph_data.pos.y - graph_data.height)
		, static_cast<int>(graph_data.pos.x + graph_data.width), static_cast<int>(graph_data.pos.y + graph_data.height), graph_data.handle, TRUE);
}