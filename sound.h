#pragma once

class SoundBase
{
private:


protected:

	int handle_;
	int play_type_;
	
	int volume_;

	bool is_loop_;
	bool is_stop_;
	bool is_play_;

	void VolumeDown(const int& kTargetVolume);

public:

	SoundBase(const char* path,int play_type, int volume,bool is_loop,const bool is_3d);

	virtual ~SoundBase();

	virtual void Update();

	void Start();

	void Stop();

	void Reset();

	const bool GetIsPlay() const;
};