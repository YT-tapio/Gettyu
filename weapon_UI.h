#pragma once

#include"normal_sub_screen.h"
#include"UI_data.h"
#include"gauss.h"
class WeaponUI
{
private:
	
	std::shared_ptr<NormalSubScreen> sub_screen_;
	std::shared_ptr<NormalSubScreen> circle_gauss_;

	std::shared_ptr<Gauss> gausser_;

	UIGraphData x_button_;
	UIGraphData y_button_;
	
	VECTOR circle_gauss_pos_;
	float circle_gauss_r_;


	void OffsetGraphSize(UIGraphData& graph_data);

	void GraphDraw(UIGraphData graph_data);

	void SetCirclePos();

public:

	WeaponUI();

	~WeaponUI();



	void Update();

	void Draw();

};