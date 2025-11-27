#pragma once
#include"DxLib.h"

class ObjectBase;
class Animation;
class Stage;

class CharacterBase : public ObjectBase
{
private:


protected:

	//stageをポインタでもっておく
	Stage* stage_;

	//characterはanimationを持っている
	std::shared_ptr<Animation> animation_;
	
	AnimationType now_anim_type;
	AnimationType before_anim_type;
	AnimationType before_before_anim_type;

	VECTOR velocity_;

	bool is_ground_;


	/// @brief 地面に接触しているか
	/// @return 
	bool IsOnGround();


public:

	CharacterBase(Stage* stage,const VECTOR& pos,const VECTOR& rot,const VECTOR& scale,const int model_handle);


	virtual ~CharacterBase() override;



	/*仮想関数*/

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw() override;

	virtual void Debug() override;




};
