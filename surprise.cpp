#include"surprise.h"
#include"fleeping.h"

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

std::shared_ptr<BaseEnemyState> EnemySurprise::ChangeState(BaseEnemy* enemy, std::shared_ptr<Player> player)
{

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