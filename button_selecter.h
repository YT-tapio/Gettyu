#pragma once
#include"select_type.h"

class ButtonSelecter
{
private:

	bool is_slide_;

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