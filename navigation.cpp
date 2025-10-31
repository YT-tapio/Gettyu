#include"navigation.h"

Navigation::Navigation()
{
	//”ƒ‚¤waypoint‚Ì’m‚è‡‚¢‚Ç‚à‚ğ‚±‚±‚É“ü‚ê‚é
	std::vector<int> neighbors;

	int num = 0;

	//1ŒÂ–Ú
	neighbors.push_back(1);
	neighbors.push_back(3);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(0, 0, 0), num, neighbors));
	num++;
	neighbors.clear();


	//2ŒÂ–Ú

	neighbors.push_back(0);
	neighbors.push_back(2);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(10, 0, 0), num, neighbors));
	num++;
	neighbors.clear();


	//3ŒÂ–Ú
	neighbors.push_back(1);
	neighbors.push_back(3);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(10, 0, 10), num, neighbors));
	num++;
	neighbors.clear();

	//4ŒÂ–Ú
	neighbors.push_back(0);
	neighbors.push_back(2);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(0, 0, 10), num, neighbors));
	num++;
	neighbors.clear();

}

Navigation::~Navigation()
{

}


void Navigation::Debug()
{

	for (const auto& way_point : way_points_)
	{
		way_point->Debug(way_points_);
	}

}