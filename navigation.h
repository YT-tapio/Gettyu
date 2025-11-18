#pragma once
#include<iostream>
#include<vector>
#include"way_point.h"

class WayPoint;

class Navigation
{
private:

	//‚¢‚ë‚ñ‚Èway_point‚ğ•Û‘¶
	std::vector<std::shared_ptr<WayPoint>> way_points_;

	void MakeWayPoint();

public:

	Navigation();

	~Navigation();


	void Debug();

	/// <summary>
	/// ˆø”‚Ì”Ô†‚Ìwaypoint‚ğ•Ô‚·
	/// </summary>
	/// <returns></returns>
	VECTOR GetWayPointPos(const int num);

	const std::vector<std::shared_ptr<WayPoint>> GetWayPoint()const { return way_points_; }



};