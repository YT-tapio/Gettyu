#pragma once

#include<vector>
#include<list>

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
	std::vector<std::shared_ptr<BaseEnemy>> enemies_;

	//baseにeffectを渡してあげる
	Effect* get_effect_ = new Effect("data/effect/MAGICALxSPIRAL/A_Salamander4.efkefc", VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 7.0f, 10.f, 300.f, FALSE);
	Effect* got_effect_ = new Effect("data/effect/NitoriBox/Explosion.efkefc", VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 6.0f, 10.f, 200.f, FALSE);

public:


	EnemyManager();

	~EnemyManager();

	void Init();

	void Update(std::shared_ptr<Player> player);


	void Draw();

	void SetDeltaTime(float delta_time);

};