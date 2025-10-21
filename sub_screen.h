#pragma once
#include<iostream>

#include"DxLib.h"
#include"movie.h"

class MociePlayer;

class BaseScreen
{
private:

	int handle_;	//MakeScreenをした時のデータの保管
	std::shared_ptr<MoviePlayer> in_line_;
public:


	BaseScreen(const int width, const int height, bool alpha);

	~BaseScreen();

	//起動てきなのつかう
	void Up();

	//シャットダウン的なの使わない
	void Down();

	void Update();

	void Draw();

};