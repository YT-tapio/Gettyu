#pragma once
#include<iostream>
#include"Dxlib.h"
#include"player.h"
#include"collision_data.h"

class Player;


class BaseEnemy
{
private:
	
protected:

	CollisionData collision_data_;

	//
	MATRIX mat_;		//vectorの集合体
	VECTOR pos_;		//ポジション
	VECTOR dir_;		//向き
	VECTOR rot_;		//回転量
	VECTOR velocity_;	//移動量
	VECTOR scale_;		//大きさ


	bool is_get_;
	int model_;

	float delta_time_;


public:

	BaseEnemy(const int model, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& dir);


	virtual ~BaseEnemy() = 0;


	virtual void Init(const VECTOR& pos, const VECTOR scale) = 0;



	virtual void Update(std::shared_ptr<Player> player) = 0;


	void Draw();

	void SetDeltaTime(float delta_time);

	void SetIsGet(bool flag);

	const VECTOR GetPos() const { return pos_; }
};

