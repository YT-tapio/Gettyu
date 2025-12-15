#include"fleeping.h"
#include"alert.h"
#include"stan.h"

EnemyFleeping::EnemyFleeping()
	: BaseEnemyState(StateName::kFleeping)
{
	outside_timer_ = std::make_shared<ConditionTimer>(kOutsideTime);
	outside_timer_->Reset();
}

EnemyFleeping::~EnemyFleeping()
{
	
}

void EnemyFleeping::Entry(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	enemy->FleepingInit(player);
}

void EnemyFleeping::Update(EnemyBase* enemy, std::shared_ptr<Player>player)
{
	enemy->Fleeping(player);
}

void EnemyFleeping::Exit(EnemyBase* enemy)
{

}

std::shared_ptr<BaseEnemyState> EnemyFleeping::ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	if (enemy->GetOnDamage()) { return std::make_shared<EnemyStan>(); }
	//playerがenemyの範囲内にいないのならやめる
	// enemyとplayerがアラート範囲内
	VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());		// enemyからplayerまでの距離
	
	//サウンド関係なく逃げる

	if (VSize(dist) <= enemy->GetAlertDist())
	{
		outside_timer_->Reset();
		return nullptr;
	}

	outside_timer_->Update();

	// 時間がたっていない
	if (!(outside_timer_->GetIsEnd()))
	{
		return nullptr;
	}
	

	// 範囲内にいない場合は警戒状態にする
	return std::make_shared<EnemyAlert>();

}