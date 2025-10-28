#include"alert.h"
#include"patrolling.h"
#include"surprise.h"

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

std::shared_ptr<BaseEnemyState> EnemyAlert::ChangeState(BaseEnemy* enemy, std::shared_ptr<Player> player)
{
	//ステートの切り替え
	//警戒し終わったら

	if (enemy->GetIsAlert())
	{
		// 警戒中
		// この時はenemyのalert距離に入ったときに逃げる

		// playerとenemyの距離をとる
		VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());		// enemyからplayerまでの距離

		//そのdistがenemyのalertとplayerのサウンドをかけ合わせた範囲内の時
		if (VSize(dist) <= (enemy->GetAlertDist() * player->GetSoundVibrationNum()))
		{
			return std::make_shared<EnemySurprise>();
		}		
	}
	else
	{
		//警戒が終わったら
		return std::make_shared<EnemyPatrolling>();
		
	}

	//どれにも該当しない場合はnullptr
	return nullptr;
}