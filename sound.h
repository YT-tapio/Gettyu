#pragma once

class Sound
{
private:

	int handle_;

public:

	Sound(const char* path);

	virtual ~Sound();

};