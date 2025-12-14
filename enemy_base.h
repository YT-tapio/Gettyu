#pragma once
#include<iostream>
#include<algorithm>
#include"Dxlib.h"
#include"player.h"
#include"effect.h"
#include"collision_data.h"
#include"enemy_FSM.h"
#include"base_enemy_state.h"
#include"condition_timer.h"
#include"alert_state.h"
#include"navigation.h"


class Player;
class Animation;
class BaseEnemyState;
class EnemyFSM;
class Stage;
class CollisionBase;
class SoundBase;

class EnemyBase
{
private:
	
	Effect* get_effect_;
	Effect* got_effect_;

	const float kAlertHigh			= 58.f;	// 警戒度(高)
	const float kAlertNormal		= 48.f;  // 警戒度(中)
	const float kAlertLow			= 38.f;  // 警戒度(低)

	const float kEngagementNormal	= 28.f;

	const float kNormalAlertTime	= 2.f;

	VECTOR near_way_point_pos_ = VGet(0, 0, 0);

	std::shared_ptr<WayPoint> DecideNextWayPoint(const VECTOR& player_pos, std::vector<std::shared_ptr<WayPoint>>way_points);

	/// @brief 吸引されているとき
	/// @param player_pos 
	/// @param way_points 
	/// @return 
	std::shared_ptr<WayPoint> DecideIsVacuumNextWayPoint(const VECTOR& player_pos, std::vector<std::shared_ptr<WayPoint>>way_points);

	float MakeWayPointScore(const VECTOR& player_pos, std::shared_ptr<WayPoint> way_point);



protected:

	CollisionData collision_data_;
	std::shared_ptr<BaseEnemyState> state_;		//一貫して最初はpatrolling
	
	std::shared_ptr<CollisionBase> coll_;

	//AI
	std::shared_ptr<EnemyFSM> fsm_;
	std::shared_ptr<Navigation> navigation_;

	std::shared_ptr<Animation> animation_;
	AnimationType now_anim_type_;            //現在のプレイヤーのアニメ～しょん
	AnimationType before_anim_type_;			//1つ前のアニメーション
	AnimationType before_before_anim_type_;	//2つ前のアニメーション

	

	//playerのインスタンスの保持
	Player* player_;

	std::shared_ptr<Stage> stage_;
	//way_pointを保存しておく
	std::shared_ptr<WayPoint> my_way_point_;
	std::shared_ptr<WayPoint> before_way_point_;

	// サウンド関連
	std::shared_ptr<SoundBase> alert_sound_;
	std::shared_ptr<SoundBase> surprise_sound_;

	ConditionTimer* alert_timer_; //警戒のタイマー
	std::shared_ptr<ConditionTimer> stan_timer_;		//stanの時間

	//
	MATRIX mat_;		//vectorの集合体
	VECTOR pos_;		//ポジション
	VECTOR dir_;		//向き
	VECTOR rot_;		//回転量
	VECTOR velocity_;	//移動量
	VECTOR scale_;		//大きさ

	VECTOR target_pos_;
	VECTOR start_pos_;

	AlertState alert_state_;	//警戒度

	bool is_get_;
	bool is_fleeping_;
	bool is_alert_;
	bool is_vacuum_;

	bool lerp_flag_;			//移動の際のラープ

	bool is_ground_;

	int model_;
	int debug_color_;

	float delta_time_;
	//各enemyによって変える
	float speed_;				//歩いているときのスピード
	float fleeping_speed_;		//逃げる時のスピード
	float alert_dist_;			//警戒の距離
	float engagement_dist_;
	float fov_;

	VECTOR GetNearWayPointPos();

	std::vector<VECTOR> GetWayPointNeighborsPos();

	std::vector<std::shared_ptr<WayPoint>> GetNeighbors();

	//fleeping時の逃げる所を決めたい
	void DecideFirstFleepingPlace(std::shared_ptr<Player> player);
	
	void DecideFleepingPlace(std::shared_ptr<Player> player, std::shared_ptr<WayPoint> way_point);

	void DecideIsVacuumFleepingPlace(std::shared_ptr<Player> player);

	std::shared_ptr<WayPoint> GetFarWayPoint(const VECTOR& pos, std::vector<std::shared_ptr<WayPoint>> way_points);


public:

	EnemyBase(const int model, const VECTOR& pos,
		const VECTOR& scale, const VECTOR& rot, Effect* effect, Effect* got_effect,
		float speed, float fleeping_speed, AlertState alert, float fov, std::shared_ptr<Stage> stage,std::shared_ptr<CollisionBase> coll, const float& stan_time);


	virtual ~EnemyBase() = 0;


	virtual void Init(const VECTOR& pos, const VECTOR scale) = 0;

	virtual void PatrollingInit(std::shared_ptr<Player> player) = 0;

	virtual void SurpriseInit(std::shared_ptr<Player> player) = 0;

	virtual void StanInit(std::shared_ptr<Player> player) = 0;

	virtual void AlertInit(std::shared_ptr<Player> player) = 0;

	virtual void FleepingInit(std::shared_ptr<Player> player) = 0;

	virtual void Update(std::shared_ptr<Player> player,bool& got) = 0;


	virtual void Patrolling() = 0;

	virtual void Surprise() = 0;

	virtual void Stan() = 0;

	virtual void Alert(std::shared_ptr<Player> player) = 0;

	virtual void Fleeping(std::shared_ptr<Player> player) = 0;

	virtual void AddAnim() = 0;

	void EffectUpdate();

	void AnimationUpdate();

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

	void SetVecuum(const bool& flag);

	VECTOR DecideNextPlace();

	bool GetIsAnimPlay() { return animation_->GetIsPlay(now_anim_type_); }

	const float GetAlertDist() const { return alert_dist_; }

	const float GetEngagementDist() const { return engagement_dist_; }

	const float GetFov() const { return fov_; }

	const bool GetIsGet() const { return is_get_; }

	const bool GetIsFleeping() const { return is_fleeping_; }

	const bool GetIsAlert() const { return is_alert_; }

	const bool GetStanIsEnd() const { return stan_timer_->GetIsEnd(); }

	const VECTOR GetPos() const { return pos_; }

	const VECTOR GetDirection() const { return dir_; }

	const CollisionData GetCollisionData() const { return collision_data_; }
};

