#include"weapon_checker.h"


WeaponChecker::WeaponChecker()
	: name_(WeaponName::kNothing)
{

}

void WeaponChecker::SetWeaponName(const WeaponName& name)
{
	name_ = name;
}