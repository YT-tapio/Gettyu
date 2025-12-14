#pragma once

class SoundSound;

class Sound2D :public SoundBase
{
private:

public:

	Sound2D(const char* path, int type,int volume, bool is_loop);

	~Sound2D() override;

	void Update() override;
};