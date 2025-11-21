#pragma once

#include"base_enemy_state.h"

class BaseEnemy;
class BaseEnemyState;
class Player;

class EnemyFleeping : public BaseEnemyState
{
private:

	const float kOutsideTime = 2.f;
	std::shared_ptr<ConditionTimer> outside_timer_;

public:

	EnemyFleeping();

	~EnemyFleeping() override;

	void Entry(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Update(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Exit(BaseEnemy* enemy) override;

	std::shared_ptr<BaseEnemyState> ChangeState(BaseEnemy* enemy, std::shared_ptr<Player> player) override;
};