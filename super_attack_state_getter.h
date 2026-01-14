#pragma once
#include"super_attack_state.h"
class SuperAttackStateGetter
{
private:

	SuperAttackState* state_;

	SuperAttackStateGetter() {}

public:
	
	
	static SuperAttackStateGetter& GetInstance()
	{
		static SuperAttackStateGetter instance;
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	SuperAttackStateGetter(const SuperAttackStateGetter&) = delete;
	SuperAttackStateGetter& operator=(const SuperAttackStateGetter&) = delete;
	
	void Set(SuperAttackState* state) { state_ = state; }

	const SuperAttackState GetState() const { return *state_; }

};