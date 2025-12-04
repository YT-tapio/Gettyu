#pragma once
#include"select_type.h"

class ButtonSelecter
{
private:

	bool is_slide_up_;
	bool is_slide_down_;
	bool is_slide_right_;
	bool is_slide_left_;
	/// <summary>
	/// è„ì¸óÕÇ≥ÇÍÇƒÇ¢ÇÈÇ©
	/// </summary>
	/// <returns></returns>
	bool IsInputUp();

	bool IsInputDown();

	bool IsInputRight();

	bool IsInputLeft();

public:

	ButtonSelecter();

	~ButtonSelecter();


	int Vertical();

	int Side();

	int Select(SelectType type);


};