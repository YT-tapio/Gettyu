#pragma once
#include"base_enemy_state.h"

class BaseEnemy;
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
	void Entry(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	/// <summary>
	/// XV
	/// </summary>
	/// <param name="enemy"></param>
	void Update(BaseEnemy* enemy, std::shared_ptr<Player> player) override;

	/// <summary>
	/// ‚µ‚ã‚¤‚è‚å‚¤‚¶‚å‚¤‚¯‚ñ
	/// </summary>
	/// <param name="enemy"></param>
	void Exit(BaseEnemy* enemy) override;
};
