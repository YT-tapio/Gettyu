#include"alert.h"
#include"patrolling.h"
#include"surprise.h"
#include"stan.h"

EnemyAlert::EnemyAlert()
	: BaseEnemyState(StateName::kAlert)
{

}


EnemyAlert::~EnemyAlert()
{

}


void EnemyAlert::Entry(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	enemy->AlertInit(player);
}

void EnemyAlert::Update(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	enemy->Alert(player);
}

void EnemyAlert::Exit(EnemyBase* enemy)
{

}

std::shared_ptr<BaseEnemyState> EnemyAlert::ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player)
{

	if (Situation::GetInstance().GetSituationName() == SituationName::kAttack) { return std::make_shared<EnemyStan>(); }

	//ステートの切り替え
	//警戒し終わったら

	if (enemy->GetIsAlert())
	{
		// 警戒中
		// この時はenemyのalert距離に入ったときに逃げる

		// playerとenemyの距離をとる
		VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());		// enemyからplayerまでの距離

		if (VSize(dist) <= (enemy->GetEngagementDist()))
		{
			return std::make_shared<EnemySurprise>();
		}

		auto sound_vibration = player->GetSoundVibrationNum();

		//そのdistがenemyのalertとplayerのサウンドをかけ合わせた範囲内の時
		if (VSize(dist) <= (enemy->GetAlertDist() * sound_vibration))
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