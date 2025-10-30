#include<iostream>
#include<vector>

#include"DxLib.h"

#include"way_point.h"
#include"Debug.h"

WayPoint::WayPoint(const VECTOR& pos, int num,std::vector<int> neighbors)
	: pos_(pos)
	, num_(num)
	,neighbors_(neighbors)
{

}

WayPoint::~WayPoint()
{

}

void WayPoint::Debug(std::vector<std::shared_ptr<WayPoint>> way_points)
{
	std::vector<VECTOR> neighbors_pos;
	int max_count = 0;
	int count = 0;
	// waypointoの可視化
	DrawSphere3D(pos_, 1.f, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawFormatString(0, Debug::GetInstance().GetCurrentNum() * Debug::GetInstance().GetFontSize(), GetColor(0, 0, 0), "know_num : ");

	//認識している数を確認
	for (const auto& neighbor : neighbors_)
	{
		max_count++;

		//認識しているやつを描画

		DrawFormatString(0, Debug::GetInstance().GetCurrentNum() * Debug::GetInstance().GetFontSize(), GetColor(0, 0, 0), "%d ", neighbor);

	}

	Debug::GetInstance().Add();
	
	//つなぐ場所の保管を行う
	for (const auto& way_point : way_points)
	{
		//way_pointの識別番号を受け取る
		auto the_num = way_point->GetNum();

		//自分が知っている番号かの判断
		for (const auto& neighbor : neighbors_)
		{
			//知っているとき
			if (neighbor == the_num)
			{
				//positionを受け取る
				neighbors_pos.push_back(way_point->GetPos());
				count++;
			}
		}

		//知ってるメンバー分補完出来たら
		if (max_count == count)
		{
			break;
		}
	}


	for (const auto& neighbor_pos : neighbors_pos)
	{
		//線でつなぐ
		DrawLine3D(pos_, neighbor_pos, GetColor(255, 255, 255));

	}


}