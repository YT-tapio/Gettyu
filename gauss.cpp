#include"gauss.h"

Gauss::Gauss()
{

}

void Gauss::Update(int handle,int pixel_width,int param)
{
	/*
	GetDrawScreenGraph(static_cast<int>(pos.x - (width * 0.5f)),
		static_cast<int>(pos.x - (height * 0.5f)),
		static_cast<int>(pos.x + (width * 0.5f)),
		static_cast<int>(pos.x + (height * 0.5f)), handle);
	*/
	
	
	

	GraphFilter(handle, DX_GRAPH_FILTER_GAUSS, pixel_width, param);

}