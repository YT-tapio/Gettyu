#pragma once

#include"base_enemy_state.h"

class EnemyBase;
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

	void Entry(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	void Update(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	void Exit(EnemyBase* enemy) override;

	std::shared_ptr<BaseEnemyState> ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player) override;
};