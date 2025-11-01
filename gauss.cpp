#include"gauss.h"

Gauss::Gauss(int pixel_width,int param)
	: pixel_width_(pixel_width)
	, param_(param)
{

}

Gauss::~Gauss()
{

}

void Gauss::Update(const VECTOR& pos, int width,int height,int handle)
{
	GetDrawScreenGraph(static_cast<int>(pos.x - (width * 0.5f)),
		static_cast<int>(pos.x - (height * 0.5f)),
		static_cast<int>(pos.x + (width * 0.5f)),
		static_cast<int>(pos.x + (height * 0.5f)), handle);

	GraphFilter(handle, DX_GRAPH_FILTER_GAUSS, pixel_width_, param_);

}