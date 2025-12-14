#pragma once

class SoundSound;

class Sound3D :public SoundBase
{
private:

	VECTOR* pos_;
	float radius_;		//•·‚±‚¦‚é”ÍˆÍ

public:

	Sound3D(const char* path, int type,int volume, bool is_loop,VECTOR* pos,float radius);

	~Sound3D() override;

	void Update() override;
};