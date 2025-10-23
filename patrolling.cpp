#include"patrolling.h"


EnemyPatrolling::EnemyPatrolling()
	:BaseEnemyState(StateName::kPatrolling)
{

}


EnemyPatrolling::~EnemyPatrolling()
{

}


void EnemyPatrolling::Entry(BaseEnemy* enemy, std::shared_ptr<Player> player)
{

}


void EnemyPatrolling::Update(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	//‚±‚Ì’†‚ÅŽU•à‚ð‚³‚¹‚Ä‚¢‚­
	enemy->Patrolling();
}

void EnemyPatrolling::Exit(BaseEnemy* enemy)
{

}