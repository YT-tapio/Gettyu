#include <fstream>
#include <sstream>
#include <string>
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
	//waypointの知り合いどもをここに入れる
	std::vector<int> neighbors;

	int num = 0;
	Load();

	/*
	//0
	neighbors.push_back(1);
	neighbors.push_back(2);
	way_points_.push_back(std::make_shared<WayPoint>(VGet(73.f, -9.53, -74.f), num, neighbors));
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

	*/
}

void Navigation::Load()
{
	const char* file_path = "data/csv/way_point2.csv";

	std::ifstream file(file_path);
	std::string line;

	if (!file)
	{
		printfDx("csvファイル読み込み失敗\n");
	}
	
	// 最初の行を飛ばす
	std::getline(file, line);

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string data;	//csvからの文字列をもらう
		VECTOR pos = VGet(0.f, 0.f, 0.f);
		int id;								//自分の識別番号
		std::vector<int> neighbors;			//waypointの知り合いどもをここに入れる

		// ID
		std::getline(ss, data, ',');
		id = std::stoi(data);

		// x
		std::getline(ss, data, ',');
		pos.x = std::stof(data);

		// y
		std::getline(ss, data, ',');
		pos.y = std::stof(data);

		// z
		std::getline(ss, data, ',');
		pos.z = std::stof(data);
		
		// 可変部分（way_pointに入れる）
		while (std::getline(ss, data, ',')) 
		{
			if (data.empty()) { break; }

			int neighbors_id = std::stoi(data);
			neighbors.push_back(neighbors_id);
		}
		way_points_.push_back(std::make_shared<WayPoint>(pos, id, neighbors));
	}


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
		printfDx("ラムダ式間違っているぞ");
	}
	
	return VGet(0, 0, 0);
}

std::shared_ptr<WayPoint> Navigation::GetWayPoint(const int num)
{
	for (auto& way_point : way_points_)
	{
		if (way_point->GetNum() == num)
		{
			return way_point;
		}
	}

	printfDx("何かが変です\n");
	return nullptr;
}

std::vector<std::shared_ptr<WayPoint>> Navigation::GetNeighbors(std::shared_ptr<WayPoint> way_point)
{
	std::vector<std::shared_ptr<WayPoint>> neighbors;

	auto nums = way_point->GetFriend();

	for (auto num : nums)
	{
		neighbors.push_back(GetWayPoint(num));
	}


	return neighbors;
}