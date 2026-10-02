#include"enemy_FSM.h"

//EnemyStateを読み込む
#include"patrolling.h"
#include"alert.h"
#include"fleeping.h"
#include"surprise.h"

EnemyFSM::EnemyFSM()
{

}


EnemyFSM::~EnemyFSM()
{

}

/*--------private----------*/

std::shared_ptr<BaseEnemyState> EnemyFSM::ChangeAlert(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player,EnemyBase* enemy)
{

	//playerからサウンドのようなものを受け取る
	auto player_sound = player->GetSoundVibrationNum();

	
	//アラートの中でさらになんかの条件なら違うのに切り替えるてきなかんじにします

	//alertに切り替えるための条件
	//enemyのalert距離にplayerのサウンドを受け取る
	float alert = enemy->GetAlertDist() * player_sound;
	
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


	
	return now_state;
	
}


std::shared_ptr<BaseEnemyState> EnemyFSM::ChangeFleeping(std::shared_ptr<BaseEnemyState> now_state, std::shared_ptr<Player> player, EnemyBase* enemy)
{
	//逃げているのなら範囲外まで
	if (enemy->GetIsFleeping())
	{
		return std::make_shared<EnemyFleeping>();
	}

	//内積(VDot)で求めましょう
	//正規化(VNorm)する
	
	//playerからenemyのvector型のdistを取る
	
	VECTOR enemy_to_player_dist = VSub(player->GetCenterPos(), enemy->GetPos());		// enemyからplayerまでの距離
	VECTOR dist_dir				= VNorm(enemy_to_player_dist);							// enemyからplayerまでのdistの正規化
	VECTOR enemy_norm_dir		= VNorm(enemy->GetDirection());							// enemyの正規化

	//dotのけっかを受け取る
	float dot = VDot(enemy_norm_dir, dist_dir);

	//角度を求める
	float rad = acosf(dot);
	float herf_fov = (enemy->GetFov() * 0.5f);

	//radがfovの半分以下かつ、視界の距離ないなら
	if (rad <= herf_fov && (enemy->GetAlertDist() >= VSize(enemy_to_player_dist)))
	{
		//printfDx("in fov\n");
		enemy->SetColor(GetColor(255, 0, 0));
		//先に驚きから
		return std::make_shared<EnemySurprise>();
	}
	else
	{
		//個々の処理はplayerが物音を立てているかの検知をおこなう
		//enemyの絶対気づく距離からplayerから発生するサウンドをかけ合わせる
		auto engage_dist = (enemy->GetEngagementDist() * player->GetSoundVibrationNum());

		//距離が接敵距離なら
		if (VSize(enemy_to_player_dist) <= engage_dist)
		{
			enemy->SetColor(GetColor(255, 0, 0));
			//逃げる
			return std::make_shared<EnemySurprise>();
		}

	}

	//今のfleeping(逃走)からアラートに代わるときalertの範囲内に敵がいるとまだ逃げる
	if (now_state->GetName() == StateName::kFleeping)
	{
		//enemyが警戒している距離内にまだいるとき
		if (VSize(enemy_to_player_dist) <= enemy->GetAlertDist())
		{
			return std::make_shared<EnemyFleeping>();
		}
		else
		{
			//警戒距離を抜け出したとき
			//enemyには警戒させる
			return std::make_shared<EnemyAlert>();
		}
	}

	//リターンされないのなら気づいていなくする
	return std::make_shared<EnemyPatrolling>();
}

std::shared_ptr<BaseEnemyState> EnemyFSM::Surprise(EnemyBase* enemy)
{
	// stateは驚きの時入ってくる
	// 入ってきたらenemyのanimationが終わっているのかを判断

	//アニメーションが再生中ならまだSurprise
	if (enemy->GetIsAnimPlay())
	{
		return std::make_shared<EnemySurprise>();
	}
	else
	{
		//アニメーションの更新が終わったら逃げさせる
		return std::make_shared<EnemyFleeping>();
	}

}


/*--------public---------*/

std::shared_ptr<BaseEnemyState> EnemyFSM::UpdateState(std::shared_ptr<BaseEnemyState> now_state,std::shared_ptr<Player> player, EnemyBase* enemy)
{
	// 最初はパトロール
	if (now_state == nullptr)
	{
		return std::make_shared<EnemyPatrolling>();
	}
	else
	{
		// 現在のステートからのチェンジ
		auto state = now_state->ChangeState(enemy, player);

		if (state != nullptr)
		{
			return state;
		}

		// stateがnullptrの返された場合
		return now_state;
	}
	
}