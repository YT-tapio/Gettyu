#pragma once

#include"normal_enemy.h"

class DanceEnemy : public NormalEnemy
{
private:

	bool is_contact_;

public:

	DanceEnemy(const TCHAR* model_path, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& dir, Effect* get_effect, Effect* got_effect,
		float speed, float fleeping_speed, AlertState alert, float fov, std::shared_ptr<Stage> stage, Navigation* navigation);

	~DanceEnemy() override;


	void Init(const VECTOR& pos, const VECTOR scale) override;

	//void Update(std::shared_ptr<Player> player, bool& got) override;

	void AddAnim() override;

	void PatrollingInit(std::shared_ptr<Player> player) override;

	void Patrolling() override;

	void PatrollingExit() override;

};
