#include <algorithm>
#include"navigation.h"

Navigation::Navigation()
{
	MakeWayPoint();



}

Navigation::~Navigation()
{

}

//private

void Navigation::MakeWayPoint()
{
	//waypoint‚Ì’m‚è‡‚¢‚Ç‚à‚ğ‚±‚±‚É“ü‚ê‚é
	std::vector<int> neighbors;

	int num = 0;

	//0
	neighbors.push_back(1);
	neighbors.push_back(2);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(73, -9.53, -74), num, neighbors));
	num++;
	neighbors.clear();


	//1
	neighbors.push_back(0);
	neighbors.push_back(6);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(-73, -9.53, -74), num, neighbors));
	num++;
	neighbors.clear();

	//2
	neighbors.push_back(0);
	neighbors.push_back(3);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(117, 0.56, -112), num, neighbors));
	num++;
	neighbors.clear();

	//3
	neighbors.push_back(4);
	neighbors.push_back(2);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(64, 0.61, -149), num, neighbors));
	num++;
	neighbors.clear();

	//4
	neighbors.push_back(3);
	neighbors.push_back(5);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(-0.35, 0.57, -163), num, neighbors));
	num++;
	neighbors.clear();

	//5
	neighbors.push_back(4);
	neighbors.push_back(6);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(-58, 0.57, -149), num, neighbors));
	num++;
	neighbors.clear();

	//6
	neighbors.push_back(1);
	neighbors.push_back(5);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(-112, 0.57, -117), num, neighbors));
	num++;
	neighbors.clear();

}


//public

void Navigation::Debug()
{

	for (const auto& way_point : way_points_)
	{
		way_point->Debug(way_points_);
	}

}

VECTOR Navigation::GetWayPointPos(const int num)
{

	if (TRUE)
	{
		for (auto way_point : way_points_)
		{
			if (way_point->GetNum() == num)
			{
				return way_point->GetPos();
			}
		}
	}
	else
	{
		std::for_each(way_points_.begin(), way_points_.end(), [&](std::shared_ptr<WayPoint> way_point) {if (way_point->GetNum() == num) { return way_point->GetPos(); }});
		printfDx("ƒ‰ƒ€ƒ_®ŠÔˆá‚Á‚Ä‚¢‚é‚¼");
	}
	
	return VGet(0, 0, 0);
}