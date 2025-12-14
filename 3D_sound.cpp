#include"DxLib.h"
#include"sound.h"
#include"3D_sound.h"

Sound3D::Sound3D(const char* path, int type,int volume, bool is_loop,VECTOR* pos,float radius)
	:SoundBase(path,type, volume, is_loop,TRUE)
{
	pos_ = pos;
	radius_ = radius;

	Set3DRadiusSoundMem(radius_, handle_);
}

Sound3D::~Sound3D()
{

}

void Sound3D::Update()
{
	Set3DPositionSoundMem(*pos_, handle_);
}