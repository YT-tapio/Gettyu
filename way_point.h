#pragma once
#include<iostream>
#include<vector>

#include"DxLib.h"
#include"sub_screen.h"

class WayPoint
{
private:

	VECTOR pos_;						// 自分のポズ
	int num_;								// 識別番号

	std::vector<int> neighbors_;			// 知っている番号たち

	//デバック表記を分かりやすく


public:

	WayPoint(const VECTOR& pos, int num, std::vector<int> neighbors);

	~WayPoint();

	void Debug(std::vector<std::shared_ptr<WayPoint>> way_point);


	const int GetNum() const { return num_; }

	const VECTOR GetPos() const { return pos_; }

	std::vector<int> GetFriend() { return neighbors_; }
};
