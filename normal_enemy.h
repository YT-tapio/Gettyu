#pragma once
#include"DxLib.h"
#include"base_enemy.h"

class Player;
class BaseEnemy;

class NormalEnemy : public BaseEnemy
{
private:

	

public:

	NormalEnemy(const char* path, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& dir, Effect* effect);


	~NormalEnemy() override;


	void Init(const VECTOR& pos,const VECTOR scale) override;


	void Update(std::shared_ptr<Player> player, bool& got) override;
	
	//void Draw() override;
};
