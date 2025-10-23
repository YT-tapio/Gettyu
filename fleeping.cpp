#include"fleeping.h"


EnemyFleeping::EnemyFleeping()
	: BaseEnemyState(StateName::kFleeping)
{

}

EnemyFleeping::~EnemyFleeping()
{

}

void EnemyFleeping::Entry(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	enemy->FleepingInit(player);
}

void EnemyFleeping::Update(BaseEnemy* enemy, std::shared_ptr<Player>player)
{
	enemy->Fleeping(player);
}

void EnemyFleeping::Exit(BaseEnemy* enemy)
{

}