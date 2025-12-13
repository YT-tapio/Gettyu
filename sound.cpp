#include"DxLib.h"
#include"sound.h"

Sound::Sound(const char* path)
{
	handle_ = LoadSoundMem(path);
}

Sound::~Sound()
{

}