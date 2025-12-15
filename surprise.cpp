#include"surprise.h"
#include"fleeping.h"
#include"stan.h"

EnemySurprise::EnemySurprise()
	:BaseEnemyState(StateName::kSurprise)
{

}

EnemySurprise::~EnemySurprise()
{

}

void EnemySurprise::Entry(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	enemy->SurpriseInit(player);
}

void EnemySurprise::Update(EnemyBase* enemy, std::shared_ptr<Player> player)
{

}

void EnemySurprise::Exit(EnemyBase* enemy)
{

}

std::shared_ptr<BaseEnemyState> EnemySurprise::ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	if (enemy->GetOnDamage()) { return std::make_shared<EnemyStan>(); }
	//surpriseのアニメーションが終わったら
	if (enemy->GetIsAnimPlay())
	{
		return nullptr;
	}
	else
	{
		//playしていない
		return std::make_shared<EnemyFleeping>();
	}
}