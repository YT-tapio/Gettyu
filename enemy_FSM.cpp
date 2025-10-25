#include"enemy_FSM.h"

//EnemyStateを読み込む
#include"patrolling.h"
#include"alert.h"
#include"fleeping.h"

EnemyFSM::EnemyFSM()
{

}


EnemyFSM::~EnemyFSM()
{

}

/*--------private----------*/

std::shared_ptr<BaseEnemyState> EnemyFSM::ChangeAlert(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player,BaseEnemy* enemy)
{

	//アラートの中でさらになんかの条件なら違うのに切り替えるてきなかんじにします

	//alertに切り替えるための条件
	//enemyのalert距離を受け取る
	float alert = enemy->GetAlertDist();

	//プレイヤーとの距離を見てその距離で判断enemyの警戒度的なのも受け取りたい
	VECTOR dist = VSub(player->GetCenterPos(), enemy->GetPos());

	//distのサイズを受け取りそのサイズがenemyのalert(警戒距離)内にいたらalertにきりかえる


	if (VSize(dist) <= alert)
	{
		enemy->SetColor(GetColor(0, 0, 0));

		//警戒中でfleepingかどうかを判断させる


		return ChangeFleeping(now_state, player, enemy);
	}
	else
	{
		enemy->SetColor(GetColor(0, 0, 0));
		return std::make_shared<EnemyPatrolling>();
		
	}

	//vacuum
	if (player->GetIsVacuum())
	{
		//playerにサウンド持たせる必要がある
	}


	
	return now_state;
	
}


std::shared_ptr<BaseEnemyState> EnemyFSM::ChangeFleeping(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player, BaseEnemy* enemy)
{
	
	if (enemy->GetIsFleeping())
	{
		return std::make_shared<EnemyFleeping>();
	}

	//内積(VDot)で求めましょう
	//正規化(VNorm)する
	
	//playerからenemyのvector型のdistを取る
	
	VECTOR enemy_to_player_dist = VSub(player->GetCenterPos(), enemy->GetPos());
	VECTOR dist_dir = VNorm(enemy_to_player_dist);							//enemyからplayerまでのdistの正規化
	VECTOR enemy_norm_dir		= VNorm(enemy->GetDirection());		//enemyの正規化

	//dotのけっかを受け取る
	float dot = VDot(enemy_norm_dir, dist_dir);

	//角度を求める
	float rad = acosf(dot);
	float herf_fov = (enemy->GetFov() * 0.5f);
	//radがfovの半分に以下ならstateを切りかえる
	if (rad <= herf_fov)
	{
		//printfDx("in fov\n");
		enemy->SetColor(GetColor(255, 0, 0));
		return std::make_shared<EnemyFleeping>();
	}
	else
	{
		const auto player_state = player->GetNowState();
		//もし、近くで走っているときは気づかせるようにする
		if (player_state == PlayerState::kRun || player_state == PlayerState::kSlowRun)
		{
			enemy->SetColor(GetColor(255, 0, 0));
			return std::make_shared<EnemyFleeping>();
		}

	}
	

	return std::make_shared<EnemyAlert>();
}

/*--------public---------*/

std::shared_ptr<BaseEnemyState> EnemyFSM::UpdateState(std::shared_ptr<BaseEnemyState> now_state,std::shared_ptr<Player> player, BaseEnemy* enemy)
{
	auto state = now_state;


	//ここの中でステートを切り替えるかの判断を行う
	//

	state = ChangeAlert(state, player, enemy);
	//state = ChangeFleeping(state, player, enemy);


	return state;
}