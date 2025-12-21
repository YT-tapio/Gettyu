#include<iostream>
#include<memory>

#define _USE_MATH_DEFINES
#include <math.h>

#include"enemy_manager.h"
#include"situation.h"
#include"Stage.h"
#include"hit_stop_timer.h"

EnemyManager::EnemyManager(std::shared_ptr<Stage> stage)
	: not_get_count_(0)
	, stage_(stage)
{
	
}

EnemyManager::~EnemyManager()
{
	delete get_effect_;
	delete got_effect_;
}

void EnemyManager::Init()
{
	const TCHAR* normal_model_path = "data/model/character/enemy/Ch14_nonPBR.mv1";

	VECTOR scale = VGet(0.07f, 0.07f,0.07f);
	
	const VECTOR kInitPos1 = VGet(10.f,		0.f,	 50.f);
	const VECTOR kInitPos2 = VGet(-80.f,		 0.f,	-13.f);
	const VECTOR kInitPos3 = VGet(-1.51f,	 2.f,	-309.f);
	const VECTOR kInitPos4 = VGet(-2.54f,	 2.f,	-720.f);
	const VECTOR kInitPos5 = VGet(141.f,		0.f,	-893.f);
	const VECTOR kInitPos6 = VGet(-117.f,	0.f,	-1043.f);

	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_path,
		kInitPos1, scale, VGet(0.f, static_cast<float>((M_PI / 180) * 70), 0.0f), get_effect_, got_effect_, 0.5f, 2.5f, AlertState::kNormal, static_cast<float>((M_PI / 180) * 100),stage_));
	
	
	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_path,
		kInitPos2, scale, VGet(0.f, static_cast<float>((M_PI / 180) * 80), 0.0f), get_effect_, got_effect_, 1.f, 2.5f, AlertState::kLow, static_cast<float>((M_PI / 180) * 100),stage_));

	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_path,
		kInitPos3, scale, VGet(0.f, static_cast<float>((M_PI / 180) * 90), 0.0f), get_effect_, got_effect_, 0.5f, 2.5f, AlertState::kHigh, static_cast<float>((M_PI / 180) * 100),stage_));
	
	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_path,
		kInitPos4, scale, VGet(0.f, static_cast<float>((M_PI / 180) * 80), 0.0f), get_effect_, got_effect_, 1.f, 2.5f, AlertState::kLow, static_cast<float>((M_PI / 180) * 100), stage_));

	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_path,
		kInitPos5, scale, VGet(0.f, static_cast<float>((M_PI / 180) * 90), 0.0f), get_effect_, got_effect_, 0.5f, 2.5f, AlertState::kHigh, static_cast<float>((M_PI / 180) * 100), stage_));

	
	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_path,
		kInitPos6, scale, VGet(0.f, static_cast<float>((M_PI / 180) * 90), 0.0f), get_effect_, got_effect_, 0.5f, 2.5f, AlertState::kHigh, static_cast<float>((M_PI / 180) * 100), stage_));
	
	//アニメーションの追加を行う

	for (auto& enemy : enemies_)
	{
		enemy->AddAnim();
	}


	
}

void EnemyManager::Update(std::shared_ptr<Player> player)
{
	if (HitStopTimer::GetInstance().CheckHitStop()) { return; }
	not_get_count_ = 0;
	//
	bool got = FALSE;

	static bool erase = FALSE;
	//bool erased = FALSE;
	static int i = 0;

	static auto it = enemies_.begin();

	for (auto itr = enemies_.begin(); itr != enemies_.end(); ++itr)
	{
		//もうすでに何かを捕まえている状況なら回さない
		if (Situation::GetInstance().GetSituationName() != SituationName::kGet)
		{
			if (!got)
			{
				(*itr)->Update(player, got);

				//ゲットした対象を消去する

				if (got)
				{
					erase = TRUE;
					Situation::GetInstance().SetRemNum(i);

					it = itr;

				}
				else
				{
					//まだゲットしていないときはカウントさせる
					i++;
				}
			}
			
			//ゲットしているのに
			if ((*itr)->GetIsGet())
			{
				if (Situation::GetInstance().GetSituationName() == SituationName::kNothing && !erase)
				{
					it = itr;
					erase = TRUE;
				}
			}


		}
		(*itr)->EffectUpdate();

		not_get_count_++;

	}
	
	

	//printfDx("%d", i);

	if (erase)
	{
		if (Situation::GetInstance().GetSituationName() != SituationName::kGet)
		{
			erase = FALSE;
			enemies_.erase(it);
			i = 0;
		}
	}
	else
	{
		i = 0;
	}

}

void EnemyManager::Draw()
{
	int i = 0;
	for (auto& enemy : enemies_)
	{
		enemy->Draw(i);
		i++;
	}
}


void EnemyManager::Debug()
{
	int i = 0;
	for (auto& enemy : enemies_)
	{
		enemy->Debug(i);
		i++;
	}

}


void EnemyManager::SetDeltaTime(float delta_time)
{
	for (auto& enemy : enemies_)
	{
		enemy->SetDeltaTime(delta_time);
	}
}


bool EnemyManager::CheckIsEnemy()
{
	return not_get_count_ != 0;
}