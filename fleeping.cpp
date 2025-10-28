#include"fleeping.h"
#include"alert.h"

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

std::shared_ptr<BaseEnemyState> EnemyFleeping::ChangeState(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	// enemyのis_fleeping_を取得
	// is_fleeping_がTRUEの場合は早期リターン
	if (enemy->GetIsFleeping()) { return nullptr; }

	// is_fleeping_が一定距離走ったとき
	// playerがenemyのalert内にいるとき

	// enemyとplayerがアラート範囲内
	VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());		// enemyからplayerまでの距離
	
	if (VSize(dist) <= enemy->GetAlertDist() * player->GetSoundVibrationNum())
	{
		//この場合initしたい
		Entry(enemy, player);
		return nullptr;
	}

	// 範囲内にいない場合は警戒状態にする
	return std::make_shared<EnemyAlert>();

}