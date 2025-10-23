#pragma once

#include<iostream>

#include"DxLib.h"
#include"base_enemy_state.h"

class BaseEnemyState;
class player;

class EnemyFSM
{
private:

	//各ステートに切り替える条件

	//
	std::shared_ptr<BaseEnemyState> ChangeAlert(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player, BaseEnemy* enemy);

	//逃走
	std::shared_ptr<BaseEnemyState> ChangeFleeping(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player, BaseEnemy* enemy);

public:

	EnemyFSM();

	~EnemyFSM();


	//どのステートに切りかえるかの判断をし、それを返してあげる
	std::shared_ptr<BaseEnemyState> UpdateState(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player, BaseEnemy* enemy);

};