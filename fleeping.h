#pragma once

#include"base_enemy_state.h"

class BaseEnemy;
class BaseEnemyState;
class Player;

class EnemyFleeping : public BaseEnemyState
{
private:


public:

	EnemyFleeping();

	~EnemyFleeping() override;

	void Entry(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Update(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Exit(BaseEnemy* enemy) override;


};