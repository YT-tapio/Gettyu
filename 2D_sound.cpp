#include"DxLib.h"
#include"sound.h"
#include"2D_sound.h"



Sound2D::Sound2D(const char* path, int type, int volume,bool is_loop)
	:SoundBase(path,type,volume,is_loop,FALSE)
{
	
}

Sound2D::~Sound2D()
{

}

void Sound2D::Update()
{
	// ƒTƒEƒ“ƒh‚ª’á‚­‚È‚éŽž‚Ìoffset
	if (is_stop_)
	{
		VolumeDown(0);
	}

	if (CheckSoundMem(handle_)) { return; }
	
	if (is_loop_)
	{
		PlaySoundMem(handle_, play_type_);
	}
	else
	{
		if (!is_play_)
		{
			PlaySoundMem(handle_, play_type_);
			is_play_ = TRUE;
		}
	}

	


	
}