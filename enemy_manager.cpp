#include<iostream>
#include<memory>
#include<vector>
#include<list>
#include"enemy_manager.h"

EnemyManager::EnemyManager()
{
	
}

EnemyManager::~EnemyManager()
{
	
}

void EnemyManager::Init()
{
	printfDx("wa\n");
	enemys.push_back(std::make_shared<NormalEnemy>("",
		VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f),VGet(10.f,0,5.0f)));

	enemys.push_back(std::make_shared<NormalEnemy>("",
		VGet(10.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), VGet(5.f, 0, 10.0f)));

	enemys.push_back(std::make_shared<NormalEnemy>("",
		VGet(-10.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), VGet(5.f, 0, 5.0f)));
}

void EnemyManager::Update(std::shared_ptr<Player> player)
{
	for (auto& enemy : enemys)
	{
		enemy->Update(player);
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys)
	{
		enemy->Draw();
	}
}

void EnemyManager::SetDeltaTime(float delta_time)
{
	for (auto& enemy : enemys)
	{
		enemy->SetDeltaTime(delta_time);
	}
}