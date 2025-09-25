#pragma once

#include"DxLib.h"
#include"base_enemy.h"
#include"normal_enemy.h"

class Player;
class BaseEnemy;
class NormalEnemy;

class EnemyManager
{
	
private:

	//ステージごとに何体のサルかを切り替えたい
	std::list<std::shared_ptr<BaseEnemy>> enemys;

public:


	EnemyManager();

	~EnemyManager();

	void Init();

	void Update(std::shared_ptr<Player> player);


	void Draw();

	void SetDeltaTime(float delta_time);

};