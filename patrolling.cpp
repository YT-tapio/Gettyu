#include"patrolling.h"
#include"surprise.h"
#include"stan.h"
#include"vector_assistant.h"

EnemyPatrolling::EnemyPatrolling()
	:BaseEnemyState(StateName::kPatrolling)
{

}


EnemyPatrolling::~EnemyPatrolling()
{

}


void EnemyPatrolling::Entry(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	enemy->PatrollingInit(player);
}


void EnemyPatrolling::Update(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	//‚±‚Ì’†‚ÅU•à‚ğ‚³‚¹‚Ä‚¢‚­
	enemy->Patrolling();
}

void EnemyPatrolling::Exit(EnemyBase* enemy)
{

}

std::shared_ptr<BaseEnemyState> EnemyPatrolling::ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player)
{
	if (enemy->GetOnDamage()) { return std::make_shared<EnemyStan>(); }
	//‹ŠE‚Éplayer‚ª‚¢‚éA‚à‚µ‚­‚ÍAâ‘Î“¦‚°‚é‹——£‚Éplayer‚ª‚¢‚éê‡

	//player‚©‚çenemy‚ÌvectorŒ^‚Ìdist‚ğæ‚é

	VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());		// enemy‚©‚çplayer‚Ü‚Å‚Ì‹——£
	VECTOR dist_dir = VNorm(dist);										// enemy‚©‚çplayer‚Ü‚Å‚Ìdist‚Ì³‹K‰»
	VECTOR enemy_norm_dir = VNorm(enemy->GetDirection());				// enemy‚Ì³‹K‰»

	dist_dir = VectorAssistant::GetPlane(dist_dir);
	enemy_norm_dir = VectorAssistant::GetPlane(enemy_norm_dir);

	//dot‚Ì‚¯‚Á‚©‚ğó‚¯æ‚é
	float dot = VDot(enemy_norm_dir, dist_dir);

	//Šp“x‚ğ‹‚ß‚é
	float rad = acosf(dot);
	float herf_fov = (enemy->GetFov() * 0.5f);

	//rad‚ªfov‚Ì”¼•ªˆÈ‰º‚©‚ÂA‹ŠE‚Ì‹——£‚È‚¢‚È‚ç
	if (rad <= herf_fov && (enemy->GetAlertDist() >= VSize(dist)))
	{
		enemy->SetColor(GetColor(255, 0, 0));
		//æ‚É‹Á‚«‚©‚ç
		return std::make_shared<EnemySurprise>();
	}

	auto sound_vibration = player->GetSoundVibrationNum();

	/*‹ŠE‚Ì”ÍˆÍŠO*/
	auto engage_dist = (enemy->GetEngagementDist() * sound_vibration);

	//‹——£‚ªÚ“G‹——£‚È‚ç
	if (VSize(dist) <= engage_dist)
	{
		enemy->SetColor(GetColor(255, 0, 0));
		//“¦‚°‚é
		return std::make_shared<EnemySurprise>();
	}

	return nullptr;
}