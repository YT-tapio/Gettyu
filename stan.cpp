#include"stan.h"
#include"alert.h"

EnemyStan::EnemyStan()
	:BaseEnemyState(StateName::kStan)
{

}

EnemyStan::~EnemyStan()
{

}

void EnemyStan::Entry(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	enemy->StanInit(player);
}

void EnemyStan::Update(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	enemy->Stan();
}

void EnemyStan::Exit(EnemyBase* enemy)
{
	
}

std::shared_ptr<BaseEnemyState> EnemyStan::ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	if (!enemy->GetStanIsEnd()) { return nullptr; }

	return std::make_shared<EnemyAlert>();
}