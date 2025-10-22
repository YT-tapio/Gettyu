#pragma once
#include"DxLib.h"
#include"base_enemy.h"

class Player;
class BaseEnemy;

class NormalEnemy : public BaseEnemy
{
private:

	VECTOR total_vel_;

	//”½“]‚·‚é‚Æ‚«‚Ì’l
	float target_rot_ = 0.f;

	//”½“]‚·‚é‚©‚Ç‚¤‚©
	bool is_return_ = FALSE;


public:


	NormalEnemy(const char* path, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect, float speed, float alert_dist);


	~NormalEnemy() override;


	void Init(const VECTOR& pos,const VECTOR scale) override;


	void Update(std::shared_ptr<Player> player, bool& got) override;
	
	void Patrolling() override;

	void Alert(std::shared_ptr<Player> player) override;

	//void Draw() override;
};
