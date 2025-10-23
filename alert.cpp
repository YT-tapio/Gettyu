#include"alert.h"


EnemyAlert::EnemyAlert()
	: BaseEnemyState(StateName::kAlert)
{

}


EnemyAlert::~EnemyAlert()
{

}


void EnemyAlert::Entry(BaseEnemy* enemy, std::shared_ptr<Player> player)
{

}

void EnemyAlert::Update(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	//printfDx("Alert");
}

void EnemyAlert::Exit(BaseEnemy* enemy)
{

}