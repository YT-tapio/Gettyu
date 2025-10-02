#pragma once
#include<iostream>
#include"Dxlib.h"
#include"player.h"
#include"effect.h"
#include"collision_data.h"

class Player;


class BaseEnemy
{
private:
	
	Effect* get_effect_;
	Effect* got_effect_;

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
		const VECTOR& scale, const VECTOR& dir,Effect* effect, Effect* got_effect);


	virtual ~BaseEnemy() = 0;


	virtual void Init(const VECTOR& pos, const VECTOR scale) = 0;



	virtual void Update(std::shared_ptr<Player> player,bool& got) = 0;

	void EffectUpdate();

	void PlayGetEffect();

	void EndGetEffect();

	void Draw(int i);

	void SetDeltaTime(float delta_time);

	void SetIsGet(bool flag);

	void SetVelocity(const VECTOR& vel);

	void AddVelocity(const VECTOR& vel);

	void SetGetEffectPos(const VECTOR& pos);

	void SetGotEffectPos(const VECTOR& pos);

	void SetPos(const VECTOR& pos);

	//ゲットされた時の位置調整
	void SetPosIsGot(const VECTOR& pos);

	const VECTOR GetPos() const { return pos_; }

	const CollisionData GetCollisionData() const { return collision_data_; }
};

