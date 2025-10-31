#pragma once
#include<iostream>
#include<vector>
#include"way_point.h"

class WayPoint;

class Navigation
{
private:

	//‚¢‚ë‚ñ‚Èway_point‚ð•Û‘¶
	std::vector<std::shared_ptr<WayPoint>> way_points_;

public:

	Navigation();

	~Navigation();


	void Debug();


};