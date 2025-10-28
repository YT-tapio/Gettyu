#pragma once

#include"base_enemy_state.h"

class EnemySurprise : public BaseEnemyState
{
private:


public:

	EnemySurprise();

	~EnemySurprise() override;

	void Entry(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Update(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Exit(BaseEnemy* enemy) override;

	std::shared_ptr<BaseEnemyState> ChangeState(BaseEnemy* enemy, std::shared_ptr<Player> player) override;
};
