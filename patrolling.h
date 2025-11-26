#pragma once
#include"base_enemy_state.h"

class EnemyBase;
class Player;

class EnemyPatrolling : public BaseEnemyState
{
private:



public:

	EnemyPatrolling();

	~EnemyPatrolling() override;

	/// <summary>
	/// ‹N“®ğŒ
	/// </summary>
	/// <param name="enemy"></param>
	void Entry(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	/// <summary>
	/// XV
	/// </summary>
	/// <param name="enemy"></param>
	void Update(EnemyBase* enemy, std::shared_ptr<Player> player) override;

	/// <summary>
	/// ‚µ‚ã‚¤‚è‚å‚¤‚¶‚å‚¤‚¯‚ñ
	/// </summary>
	/// <param name="enemy"></param>
	void Exit(EnemyBase* enemy) override;

	std::shared_ptr<BaseEnemyState> ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player) override;
};
