#pragma once
#include"DxLib.h"
#include"enemy_base.h"


class Player;
class EnemyBase;
class CollisionBase;
class CollisionSphere;

class NormalEnemy : public EnemyBase
{
private:

	// ƒ‰ƒWƒAƒ“‚É‚µ‚½Žž‚Ì1“x‚Ì’l
	const float kRad = static_cast<float>(M_PI / 180);
	const float kReverceRad = kRad * 180;		//”½“]‚Ì’l

	const float kGravity = 0.75f;

	const float kWaitTime = 2.5f;

	VECTOR total_vel_;

	ConditionTimer* wait_timer_;

	std::shared_ptr<CollisionBase> gravity_check_coll_;

	//”½“]‚·‚é‚Æ‚«‚Ì’l
	float target_rot_ = 0.f;

	float lerp_timer_;
	//”½“]‚·‚é‚©‚Ç‚¤‚©
	bool is_return_ = FALSE;

	float fall_speed_;


	void DecideNextPos();

	bool CheckIsGound();

	void Gravity();

public:


	NormalEnemy(const TCHAR* model_path, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect,
		float speed, float fleeping_speed, AlertState alert, float fov, std::shared_ptr<Stage> stage);


	~NormalEnemy() override;

	

	void Init(const VECTOR& pos,const VECTOR scale) override;


	void PatrollingInit(std::shared_ptr<Player> player) override;

	void SurpriseInit(std::shared_ptr<Player> player) override;

	void AlertInit(std::shared_ptr<Player> player)override;

	void FleepingInit(std::shared_ptr<Player> player) override;

	void AddAnim() override;

	void Update(std::shared_ptr<Player> player, bool& got) override;
	
	void Patrolling() override;

	void Surprise() override;

	void Alert(std::shared_ptr<Player> player) override;

	void Fleeping(std::shared_ptr<Player> player) override;
	//void Draw() override;
};
