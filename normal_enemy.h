#pragma once
#include"DxLib.h"
#include"base_enemy.h"


class Player;
class BaseEnemy;

class NormalEnemy : public BaseEnemy
{
private:

	

	// ƒ‰ƒWƒAƒ“‚É‚µ‚½Žž‚Ì1“x‚Ì’l
	const float kRad = static_cast<float>(M_PI / 180);
	const float kReverceRad = kRad * 180;		//”½“]‚Ì’l


	VECTOR total_vel_;

	//”½“]‚·‚é‚Æ‚«‚Ì’l
	float target_rot_ = 0.f;

	//”½“]‚·‚é‚©‚Ç‚¤‚©
	bool is_return_ = FALSE;


public:


	NormalEnemy(int model, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect, float speed, float fleeping_speed, AlertState alert, float engagement_dist,float fov);


	~NormalEnemy() override;


	void Init(const VECTOR& pos,const VECTOR scale) override;


	void PatrollingInit(std::shared_ptr<Player> player) override;

	void SurpriseInit(std::shared_ptr<Player> player) override;

	void AlertInit(std::shared_ptr<Player> player)override;

	void FleepingInit(std::shared_ptr<Player> player) override;


	void Update(std::shared_ptr<Player> player, bool& got) override;
	
	void Patrolling() override;

	void Surprise() override;

	void Alert(std::shared_ptr<Player> player) override;

	void Fleeping(std::shared_ptr<Player> player) override;
	//void Draw() override;
};
