#include<iostream>
#include<memory>

#define _USE_MATH_DEFINES
#include <math.h>

#include"enemy_manager.h"
#include"situation.h"

EnemyManager::EnemyManager()
{
	
}

EnemyManager::~EnemyManager()
{
	
	delete get_effect_;
	delete got_effect_;
}

void EnemyManager::Init()
{
	int normal_model_data = MV1LoadModel("data/model/character/enemy/Ch14_nonPBR.mv1");

	//printfDx("wa\n");
	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_data,
		VGet(10.f, 0.f, 50.f), VGet(0.05f, 0.05f, 0.05f), VGet(0.f, static_cast<float>((M_PI / 180) * 70), 0.0f), get_effect_, got_effect_, 2.f, 4.f, 40.f, 20.f, static_cast<float>((M_PI / 180) * 100)));
	/*
	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_data,
		VGet(50.f, 1.5f, 10.f), VGet(0.05f, 0.05f, 0.05f), VGet(0.f, static_cast<float>((M_PI / 180) * 80), 0.0f), get_effect_, got_effect_,2.f,5.f, 40.f, 20.f, static_cast<float>((M_PI / 180) * 100)));

	enemies_.push_back(std::make_shared<NormalEnemy>(normal_model_data,
		VGet(20.f, 3.f, 5.f), VGet(0.05f, 0.05f, 0.05f), VGet(0.f, static_cast<float>((M_PI / 180) * 90), 0.0f), get_effect_, got_effect_,0.5f, 2.5f,40.f, 20.f, static_cast<float>((M_PI / 180) * 100)));
	*/

	//アニメーションスピード
	const float kAnimationWalkSpeed = 3.f;
	const float kAnimationSurpriseSpeed = 3.f;
	//各アニメーションを生成する
	
	AnimationData walk;
	AnimationData surprise;
	


	//アニメーションをロード
	Load(walk, "data/model/character/enemy/animation/Walking.mv1", AnimationType::kWalk, normal_model_data, 1, kAnimationWalkSpeed);
	Load(surprise, "data/model/character/enemy/animation/Joyful_Jump.mv1", AnimationType::kSurprise, normal_model_data, 1, kAnimationSurpriseSpeed);
	//アニメーションの追加を行う

	for (auto& enemy : enemies_)
	{
		enemy->AddAnim(walk);
		enemy->AddAnim(surprise);
	}


	
}

void EnemyManager::Update(std::shared_ptr<Player> player)
{
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
	return enemies_.empty();
}