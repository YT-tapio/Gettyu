#include"DxLib.h"
#include"mask.h"

MaskCreator::MaskCreator()
{

}

void MaskCreator::CreatMask()
{
	CreateMaskScreen();
}

void MaskCreator::DeleteMask()
{
	DeleteMaskScreen();
}

void MaskCreator::Up(const int handle,const bool is_in)
{
	if (is_in_ != is_in)
	{
		is_init_ = TRUE;
		is_in_ = is_in;
		SetMaskReverseEffectFlag(is_in_);
	}
	else
	{
		if (!is_init_)
		{
			is_init_ = TRUE;
			SetMaskReverseEffectFlag(is_in_);
		}
	}


	SetMaskScreenGraph(handle);
	SetUseMaskScreenFlag(TRUE);
}


void MaskCreator::Down()
{
	SetUseMaskScreenFlag(FALSE);
}