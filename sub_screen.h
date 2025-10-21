#pragma once
#include<iostream>

#include"DxLib.h"
#include"movie.h"
#include"base_sub_screen.h"

class BaseSubScreen;
class MociePlayer;

class ConcentrationLine : public BaseSubScreen
{
private:

	std::shared_ptr<MoviePlayer> in_line_;
	
public:


	ConcentrationLine(const int width, const int height, bool alpha);

	~ConcentrationLine() override;

	
	void Update() override;



};