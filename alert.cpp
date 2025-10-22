#include"alert.h"


EnemyAlert::EnemyAlert()
	: BaseEnemyState(StateName::kAlert)
{

}


EnemyAlert::~EnemyAlert()
{

}


void EnemyAlert::Entry(BaseEnemy* enemy)
{

}

void EnemyAlert::Update(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	printfDx("yeah");
}

void EnemyAlert::Exit(BaseEnemy* enemy)
{

}