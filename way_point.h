#pragma once
#include<iostream>
#include<vector>

#include"DxLib.h"

class WayPoint
{
private:

	VECTOR pos_;						// ©•ª‚Ìƒ|ƒY
	int num_;								// ¯•Ê”Ô†

	std::vector<int> neighbors_;			// ’m‚Á‚Ä‚¢‚é”Ô†‚½‚¿
public:

	WayPoint(const VECTOR& pos, int num, std::vector<int> neighbors);

	~WayPoint();

	void Debug(std::vector<std::shared_ptr<WayPoint>> way_point);


	const int GetNum() const { return num_; }

	const VECTOR GetPos() const { return pos_; }

	const std::vector<int> GetFriend() const { return neighbors_; }
};
