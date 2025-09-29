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
	
}

void EnemyManager::Init()
{
	//printfDx("wa\n");
	enemys.push_back(std::make_shared<NormalEnemy>("",
		VGet(10.f, 0.f, 50.f), VGet(0.f, 0.f, 0.f),VGet(10.f,0,5.0f)));

	enemys.push_back(std::make_shared<NormalEnemy>("",
		VGet(50.f, 1.5f, 10.f), VGet(0.f, 0.f, 0.f), VGet(5.f, 0, 10.0f)));

	enemys.push_back(std::make_shared<NormalEnemy>("",
		VGet(20.f, 3.f, 5.f), VGet(0.f, 0.f, 0.f), VGet(5.f, 0, 5.0f)));
}

void EnemyManager::Update(std::shared_ptr<Player> player)
{
	//
	bool got = FALSE;
	for (auto& enemy : enemys)
	{

		//‚à‚¤‚·‚Å‚É‰½‚©‚ð•ß‚Ü‚¦‚Ä‚¢‚éó‹µ‚È‚ç‰ñ‚³‚È‚¢

		if (Situation::GetInstance().GetSituationName() == SituationName::kNothing)
		{
			if (!got)
			{
				enemy->Update(player, got);
			}

			if (got)
			{
				printfDx("got\n");
			}
		}

		

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