#pragma once
#include"base_enemy_state.h"


class EnemyStan : public BaseEnemyState
{
private:


public:

	EnemyStan();

	~EnemyStan() override;

	void Entry(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	void Update(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	void Exit(EnemyBase* enemy) override;

	std::shared_ptr<BaseEnemyState> ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player) override;

};
