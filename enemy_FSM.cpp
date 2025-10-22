#include"enemy_FSM.h"

//EnemyStateを読み込む
#include"patrolling.h"
#include"alert.h"

EnemyFSM::EnemyFSM()
{

}


EnemyFSM::~EnemyFSM()
{

}

/*--------private----------*/

std::shared_ptr<BaseEnemyState> EnemyFSM::ChangeAlert(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player,BaseEnemy* enemy)
{
	//alertに切り替えるための条件
	//enemyのalert距離を受け取る
	float alert = enemy->GetAlertDist();

	//プレイヤーとの距離を見てその距離で判断enemyの警戒度的なのも受け取りたい
	VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());

	//distのサイズを受け取りそのサイズがenemyのalert(警戒距離)内にいたらalertにきりかえる
	
	
	if (VSize(dist) <= alert)
	{
		return std::make_shared<EnemyAlert>();
	}
	else
	{
		return std::make_shared<EnemyPatrolling>();
	}
	
	return now_state;
	
}



/*--------public---------*/

std::shared_ptr<BaseEnemyState> EnemyFSM::UpdateState(std::shared_ptr<BaseEnemyState> now_state,std::shared_ptr<Player> player, BaseEnemy* enemy)
{
	auto state = now_state;


	//ここの中でステートを切り替えるかの判断を行う
	//

	state = ChangeAlert(state, player, enemy);



	return state;
}