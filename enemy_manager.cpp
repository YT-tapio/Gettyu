#include<iostream>
#include<memory>
#include<vector>
#include<list>
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
	//printfDx("wa\n");
	enemys.push_back(std::make_shared<NormalEnemy>("data/model/character/Ch14_nonPBR.mv1",
		VGet(10.f, 0.f, 50.f), VGet(0.05f, 0.05f, 0.05f),VGet(10.f,0,5.0f),get_effect_,got_effect_));

	enemys.push_back(std::make_shared<NormalEnemy>("data/model/character/Ch14_nonPBR.mv1",
		VGet(50.f, 1.5f, 10.f), VGet(0.05f, 0.05f, 0.05f), VGet(5.f, 0, 10.0f), get_effect_, got_effect_));

	enemys.push_back(std::make_shared<NormalEnemy>("data/model/character/Ch14_nonPBR.mv1",
		VGet(20.f, 3.f, 5.f), VGet(0.05f, 0.05f, 0.05f), VGet(5.f, 0, 5.0f), get_effect_, got_effect_));
}

void EnemyManager::Update(std::shared_ptr<Player> player)
{
	//
	bool got = FALSE;

	static bool erase = FALSE;
	//bool erased = FALSE;
	static int i = 0;

	

	for (auto& enemy : enemys)
	{
		//もうすでに何かを捕まえている状況なら回さない
		if (Situation::GetInstance().GetSituationName() == SituationName::kNothing)
		{
			if (!got)
			{
				enemy->Update(player, got);

				//ゲットした対象を消去する

				if (got)
				{
					erase = TRUE;
					Situation::GetInstance().SetRemNum(i);
				}
				else
				{
					//まだゲットしていないときはカウントさせる
					i++;
				}

			}

		}
		
		enemy->EffectUpdate();

	}

	

	if (erase)
	{
		if (Situation::GetInstance().GetSituationName() != SituationName::kGet)
		{
			erase = FALSE;

			enemys.erase(enemys.begin());
			printfDx("%d\n", enemys.begin());
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
	for (auto& enemy : enemys)
	{
		enemy->Draw(i);
		i++;
	}
}

void EnemyManager::SetDeltaTime(float delta_time)
{
	for (auto& enemy : enemys)
	{
		enemy->SetDeltaTime(delta_time);
	}
}