#include"surprise.h"

EnemySurprise::EnemySurprise()
	:BaseEnemyState(StateName::kSurprise)
{

}

EnemySurprise::~EnemySurprise()
{

}

void EnemySurprise::Entry(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	enemy->SurpriseInit(player);
}

void EnemySurprise::Update(BaseEnemy* enemy, std::shared_ptr<Player> player)
{

}

void EnemySurprise::Exit(BaseEnemy* enemy)
{

}