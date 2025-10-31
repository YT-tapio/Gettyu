#pragma once
#include"normal_sub_screen.h"
#include"UI_data.h"

class WeaponUI
{
private:
	
	std::shared_ptr<NormalSubScreen> sub_screen_;

	UIGraphData x_button_;
	UIGraphData y_button_;

	void OffsetGraphSize(UIGraphData& graph_data);

	void GraphDraw(UIGraphData graph_data);

public:

	WeaponUI();

	~WeaponUI();



	void Update();

	void Draw();

};