#pragma once
#include"DxLib.h"


class ObjectBase;
class CollisionBase;
class Animation;
class Stage;

class CharacterBase : public ObjectBase
{
private:


protected:

	//stageをポインタでもっておく
	Stage* stage_;

	//当たり判定
	std::shared_ptr<CollisionBase> coll_;

	//characterはanimationを持っている
	std::shared_ptr<Animation> animation_;
	
	AnimationType now_anim_type;
	AnimationType before_anim_type;
	AnimationType before_before_anim_type;

	VECTOR velocity_;
	VECTOR dir_;

	bool is_ground_;
	bool is_move_;

	float fall_speed_;

	

	/// @brief 地面に接触しているか
	void CheckIsGround();


public:

	CharacterBase(Stage* stage,std::shared_ptr<CollisionBase> coll,const VECTOR& pos,const VECTOR& rot,const VECTOR& scale,const char* path);


	virtual ~CharacterBase() override;



	/*仮想関数*/

	virtual void SetDeltaTime() override;

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw() override;

	virtual void Debug() override;




};
