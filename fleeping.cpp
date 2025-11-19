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
	
	//player‚ªenemy‚Ì”ÍˆÍ“à‚É‚¢‚È‚¢‚Ì‚È‚ç‚â‚ß‚é

	// enemy‚Æplayer‚ªƒAƒ‰[ƒg”ÍˆÍ“à
	VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());		// enemy‚©‚çplayer‚Ü‚Å‚Ì‹——£
	
	if (VSize(dist) <= enemy->GetAlertDist() * player->GetSoundVibrationNum())
	{
		//‚±‚Ìê‡init‚µ‚½‚¢
		Entry(enemy, player);
		return nullptr;
	}

	// ”ÍˆÍ“à‚É‚¢‚È‚¢ê‡‚ÍŒx‰úó‘Ô‚É‚·‚é
	return std::make_shared<EnemyAlert>();

}