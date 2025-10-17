#include"sky_dom.h"

SkyDom::SkyDom(const  char* path,const VECTOR& pos)
	: pos_(pos)
{
	model_data_ = MV1LoadModel(path);
	if (model_data_ == -1)
	{
		printfDx("ÉÇÉfÉãì«Ç›çûÇ›é∏îs");
	}

	MV1SetScale(model_data_, VGet(0.8f, 0.8f, 0.8f));
	MV1SetPosition(model_data_,pos_);
}

SkyDom::~SkyDom()
{
	MV1DeleteModel(model_data_);
}

void SkyDom::Draw()
{
	MV1DrawModel(model_data_);
}