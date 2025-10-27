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
	enemy->AlertInit(player);
}

void EnemyAlert::Update(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	enemy->Alert(player);
}

void EnemyAlert::Exit(BaseEnemy* enemy)
{

}