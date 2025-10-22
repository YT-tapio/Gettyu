#pragma once
#include"base_enemy.h"

class BaseEnemy;

enum class StateName
{
	kNothing,			// ‚È‚ñ‚à‚È‚µ
	kPatrolling,			// ‚³‚ñ‚Û(Œ©‚Â‚©‚Á‚Ä‚È‚¢)
	kAlert,				// Œx‰úƒ‚[ƒh
	kAttack,				// UŒ‚
	kFleeping,			// “¦‘–’†
	kGet					// •ß‚Ü‚Á‚½
};

class BaseEnemyState
{
private:

	StateName name_;

public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="state_name">‚È‚ñ‚Ìstate‚©</param>
	BaseEnemyState(StateName name);

	virtual ~BaseEnemyState() = 0;

	/// <summary>
	/// ‚»‚Ìstate‚É‚È‚éğŒ
	/// </summary>
	virtual void Entry(BaseEnemy* enemy) = 0;


	/// <summary>
	/// enemy‚Ì’†‚É‚ ‚é‚»‚ê‚¼‚ê‚Ìupdate‚ğ“Ç‚ñ‚Å‚ ‚°‚é
	/// </summary>
	/// <param name="enemy"></param>
	virtual void Update(BaseEnemy* enemy) = 0;

	/// <summary>
	/// I—¹ğŒ
	/// </summary>
	virtual void Exit(BaseEnemy* enemy) = 0;

};
