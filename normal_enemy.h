#pragma once
#include"DxLib.h"
#include"enemy_base.h"

class Player;
class EnemyBase;
class ColliderBase;
class CollisionSphere;

class NormalEnemy : public EnemyBase
{
private:

	const float kGravity = 0.75f;

	const float kWaitTime = 2.5f;

	std::shared_ptr<ColliderBase> gravity_check_coll_;

	//”½“]‚·‚é‚Æ‚«‚Ì’l
	float target_rot_ = 0.f;

	float lerp_timer_;
	//”½“]‚·‚é‚©‚Ç‚¤‚©
	bool is_return_ = FALSE;
	bool is_vacuum_init_;

	float fall_speed_;


	

	bool CheckIsGound();

	void Gravity();

protected:

	VECTOR total_vel_;

	ConditionTimer* wait_timer_;

	void DecideNextPos();

	AnimationType ChageAnimType(AnimationType now,AnimationType next);

public:


	NormalEnemy(const TCHAR* model_path, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect,
		float speed, float fleeping_speed, AlertState alert, float fov, std::shared_ptr<Stage> stage,Navigation* navigation);


	virtual ~NormalEnemy() override;

	

	virtual void Init(const VECTOR& pos,const VECTOR scale) override;

	virtual void PatrollingInit(std::shared_ptr<Player> player) override;

	void SurpriseInit(std::shared_ptr<Player> player) override;

	void StanInit(std::shared_ptr<Player> player) override;

	void AlertInit(std::shared_ptr<Player> player)override;

	void FleepingInit(std::shared_ptr<Player> player) override;

	virtual void AddAnim() override;

	virtual void Update(std::shared_ptr<Player> player, bool& got) override;
	
	virtual void Patrolling() override;

	void Surprise() override;

	void Stan() override;

	void Alert(std::shared_ptr<Player> player) override;

	void Fleeping(std::shared_ptr<Player> player) override;

	virtual void PatrollingExit() override;

	virtual void SurpriseExit() override;

	virtual void StanExit() override;

	virtual void AlertExit() override;

	virtual void FleepingExit() override;

};
