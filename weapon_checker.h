#pragma once
#include"weapon_base.h"
class WeaponChecker
{
private:

	WeaponName name_;

	WeaponChecker();

public:

	static WeaponChecker& GetInstance()
	{
		static WeaponChecker instance;	//性的変数としてインスタンスを定義
		return instance;
	}

	//コピーコンストラクタと代入演算子を消去
	WeaponChecker(const WeaponChecker&) = delete;
	WeaponChecker& operator = (const WeaponChecker&) = delete;

	void SetWeaponName(const WeaponName& name);

	const WeaponName GetName() const { return name_; }

};