#pragma once
#include"enemy_base.h"


class EnemyBase;
class Player;

enum class StateName
{
	kNothing,			// なんもなし
	kPatrolling,			// さんぽ(見つかってない)
	kAlert,				// 警戒モード
	kSurprise,			//プレイヤーに気づく1
	kAttack,				// 攻撃
	kFleeping,			// 逃走中
	kGet					// 捕まった
};

class BaseEnemyState
{
private:

	StateName name_;

public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="state_name">なんのstateか</param>
	BaseEnemyState(StateName name);

	virtual ~BaseEnemyState() = 0;

	/// <summary>
	/// そのstateになる条件
	/// </summary>
	virtual void Entry(EnemyBase* enemy, std::shared_ptr<Player> player) = 0;


	/// <summary>
	/// enemyの中にあるそれぞれのupdateを読んであげる
	/// </summary>
	/// <param name="enemy"></param>
	virtual void Update(EnemyBase* enemy, std::shared_ptr<Player> player) = 0;

	/// <summary>
	/// 終了条件
	/// </summary>
	virtual void Exit(EnemyBase* enemy) = 0;


	virtual std::shared_ptr<BaseEnemyState> ChangeState(EnemyBase* enemy, std::shared_ptr<Player> player) = 0;

	const StateName GetName() const { return name_; }

};
