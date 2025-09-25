#include"wizard_staff.h"

WizardStaff::WizardStaff()
	:WeaponBase()
{
	scale_ = VGet(kScale, kScale, kScale);
	model_ = MV1LoadModel("data/model/weapon/use_path/Wizard_Staff.mv1");
	r_ = 0;
	bone_path_ = 0;
}

WizardStaff::~WizardStaff()
{

}

void WizardStaff::Update()
{

}