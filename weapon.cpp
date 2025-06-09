#include"DxLib.h"
#include"weapon.h"

void Weapon::Draw()
{
	
	//VECTOR scale_dir = VTransformSR(scale_dir, mat_);
	VECTOR pos_ = VGet(0, 0, 0);
	
	VTransform(pos_, mat_);

	MATRIX pos_mat = MGetTranslate(pos_);
	//MATRIX scale_dir_mat = MMult(MGetScale(scale_dir), MGetScale(scale_));

	MATRIX model_mat = MMult(MGetScale(scale_), mat_);

	MV1SetMatrix(model_, model_mat);
	MV1DrawModel(model_);
}