#pragma once


//“®‰æ‘fŞ‚ğ—¬‚·Û‚É•K—v‚È“z
class MoviePlayer
{
private:

	int handle_;
	bool loop_;

public:

	MoviePlayer(const char* path,bool loop);

	~MoviePlayer();


	void Play();

	void Draw();

};
