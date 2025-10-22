#pragma once

#include<iostream>

#include"DxLib.h"
#include"base_enemy_state.h"

class BaseEnemyState;

class EnemyFSM
{
private:


public:

	EnemyFSM();

	~EnemyFSM();


	//どのステートに切りかえるかの判断をし、それを返してあげる
	std::shared_ptr<BaseEnemyState> UpdateState(std::shared_ptr<BaseEnemyState> now_state);

};