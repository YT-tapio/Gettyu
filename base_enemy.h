#pragma once
#include<iostream>
#include"Dxlib.h"
#include"player.h"
#include"effect.h"
#include"collision_data.h"
#include"enemy_FSM.h"
#include"base_enemy_state.h"

class Player;
class BaseEnemyState;
class EnemyFSM;

class BaseEnemy
{
private:
	
	Effect* get_effect_;
	Effect* got_effect_;

protected:

	CollisionData collision_data_;
	std::shared_ptr<BaseEnemyState> state_;		//一貫して最初はpatrolling
	std::shared_ptr<EnemyFSM> fsm_;

	//
	MATRIX mat_;		//vectorの集合体
	VECTOR pos_;		//ポジション
	VECTOR dir_;		//向き
	VECTOR rot_;		//回転量
	VECTOR velocity_;	//移動量
	VECTOR scale_;		//大きさ

	bool is_get_;
	bool is_fleeping_;


	int model_;
	int debug_color_;

	float delta_time_;
	//各enemyによって変える
	float speed_;
	float alert_dist_;
	float fov_;


public:

	BaseEnemy(const int model, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& rot, Effect* effect, Effect* got_effect, float speed, float alert_dist, float fov);


	virtual ~BaseEnemy() = 0;


	virtual void Init(const VECTOR& pos, const VECTOR scale) = 0;

	virtual void FleepingInit(std::shared_ptr<Player> player) = 0;

	virtual void Update(std::shared_ptr<Player> player,bool& got) = 0;


	virtual void Patrolling() = 0;

	virtual void Alert(std::shared_ptr<Player> player) = 0;

	virtual void Fleeping(std::shared_ptr<Player> player) = 0;

	void EffectUpdate();

	void PlayGetEffect();

	void EndGetEffect();

	void Draw(int i);

	void DrawFov();

	void Debug(int i);

	void SetColor(int color);

	void SetDeltaTime(float delta_time);

	void SetIsGet(bool flag);

	void SetVelocity(const VECTOR& vel);

	void AddVelocity(const VECTOR& vel);

	void SetGetEffectPos(const VECTOR& pos);

	void SetGotEffectPos(const VECTOR& pos);

	void SetPos(const VECTOR& pos);

	//ゲットされた時の位置調整
	void SetPosIsGot(const VECTOR& pos);

	const float GetAlertDist() const { return alert_dist_; }

	const float GetFov() const { return fov_; }

	const bool GetIsGet() const { return is_get_; }

	const bool GetIsFleeping() const { return is_fleeping_; }

	const VECTOR GetPos() const { return pos_; }

	const VECTOR GetDirection() const { return dir_; }

	const CollisionData GetCollisionData() const { return collision_data_; }
};

