#include"DxLib.h"
#include"sound.h"
#include"FPS.h"

SoundBase::SoundBase(const char* path,int type, int volume,bool is_loop,bool is_3d)
{
	play_type_ = type;
	is_loop_ = is_loop;
	is_stop_ = FALSE;
	is_play_ = FALSE;
	volume_ = volume;

	SetCreate3DSoundFlag(is_3d);

	handle_ = LoadSoundMem(path);
	
	if (handle_ == -1)
	{
		printfDx("ÉTÉEÉìÉhÇÃì«Ç›çûÇ›é∏îs");
	}
	
	ChangeVolumeSoundMem(volume_, handle_);
}
	

SoundBase::~SoundBase()
{
	DeleteSoundMem(handle_);
}


void SoundBase::VolumeDown(const int& kTargetVolume)
{
	const float kOffsetSpeed = 1.f;
	
	volume_ -= kOffsetSpeed * FPS::GetInstance().GetDeltaTime();

	if (volume_ < kTargetVolume) { volume_ = kTargetVolume; }
	ChangeVolumeSoundMem(volume_, handle_);
	if (volume_ == 0) { StopSoundMem(handle_); }
}


void SoundBase::Update()
{

}

void SoundBase::Start()
{
	is_stop_ = FALSE;
}

void SoundBase::Stop()
{
	is_stop_ = TRUE;
}

void SoundBase::Reset()
{
	StopSoundMem(handle_);
	is_play_ = FALSE;
}

const bool SoundBase::GetIsPlay() const
{
	return is_play_;
}
