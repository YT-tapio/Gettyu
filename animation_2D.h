#pragma once
#include<vector>

class Animation2D
{
private:

	// 
	std::vector<std::vector<int>> handle_;

public:

	Animation2D(const char* path,const int x_num,const int y_num,const int x_size,const int y_size);

	~Animation2D();


};