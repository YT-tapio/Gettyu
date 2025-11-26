#pragma once
#include"base_enemy_state.h"

class EnemyBase;
class BaseEnemyState;
class Player;

class EnemyAlert : public BaseEnemyState
{
private:


public:

	EnemyAlert();

	~EnemyAlert() override;

	void Entry(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	void Update(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	void Exit(EnemyBase* enemy) override;

	std::shared_ptr<BaseEnemyState> ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player) override;

};