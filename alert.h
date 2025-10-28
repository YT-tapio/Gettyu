#pragma once
#include"base_enemy_state.h"

class BaseEnemy;
class BaseEnemyState;
class Player;

class EnemyAlert : public BaseEnemyState
{
private:


public:

	EnemyAlert();

	~EnemyAlert() override;

	void Entry(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Update(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	void Exit(BaseEnemy* enemy) override;

	std::shared_ptr<BaseEnemyState> ChangeState(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

};