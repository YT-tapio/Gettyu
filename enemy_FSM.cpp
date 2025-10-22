#include"enemy_FSM.h"

EnemyFSM::EnemyFSM()
{

}


EnemyFSM::~EnemyFSM()
{

}


std::shared_ptr<BaseEnemyState> EnemyFSM::UpdateState(std::shared_ptr<BaseEnemyState> now_state)
{
	auto state = now_state;


	//ここの中でステートを切り替えるあの判断を行う



	return state;
}